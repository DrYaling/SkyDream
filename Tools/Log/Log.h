#ifndef _SD_LOG_H
#define _SD_LOG_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include <map>
#include <thread>
#include <boost/filesystem.hpp>
#define LOG_IN_THREAD 0
//#define LOG_TYPE_AS_TITLE
#define LOG_SWITCH 1
#define LOG_TO_FILE
#ifdef LOG_TO_FILE
#define logToFile 1
#define splitLogFileByType 1
#else
#define logToFile 0
//#define splitLogFileByType 0
#endif
#ifdef splitLogFileByType
#if splitLogFileByType
#define globalLogFilePath "../Logs/"
#else
#define globalLogFilePath "../Logs/log.txt"
#endif
#endif
#ifdef WIN32
#include "windows.h"
#define sleep(x) Sleep(x)
#endif
static std::map<const char*, std::ofstream*> _logFiles = std::map<const char*, std::ofstream*>();
static std::mutex _logLock;
static std::string buff;
static std::map<std::string*, const char*> logContents;
static std::map<std::string*, const char*> logContentsSwap;
static char cbuffer[80 * 2048] = { 0 };
static bool logInitilized = false;
enum LoggerType
{
	LOGGER_LOG = 0,
	LOGGER_WARN = 1,
	LOGGER_ERROR = 2
};
#if LOG_SWITCH
#define LogFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_LOG, logType, format, __VA_ARGS__)
#define Log(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_LOG, logType, format)
#define LogWarningFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_WARN, logType, format, __VA_ARGS__)
#define LogWarning(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_WARN, logType, format)
#define LogErrorFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_ERROR, logType, format, __VA_ARGS__)
#define LogError(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_ERROR, logType, format)
#else
#define LogFormat(logType,format, ...) 
#define Log(logType,format)
#define LogWarningFormat(logType,format, ...) 
#define LogWarning(logType,format) 
#define LogErrorFormat(logType,format, ...)
#define LogError(logType,format) 
#endif
class GlobalLogger
{
public:
	static void Init()
	{
		if (logInitilized)
			return;
#if LOG_IN_THREAD
		static std::thread logThread(&GlobalLogger::LogThreaded);
		logThread.detach();
#endif
		logInitilized = true;
	}
	static void Clear(const char* logType = nullptr)
	{
		if (nullptr != logType)
		{
			auto itr = _logFiles.find(logType);
			if (itr != _logFiles.end())
			{
				std::ofstream*f = itr->second;
				f->close();
				delete f;
				_logFiles.erase(itr);
			}
		}
		else
		{
			for (auto itr : _logFiles)
			{
				std::ofstream* f = itr.second;
				f->close();
				delete f;
			}
			_logFiles.clear();
		}
	}
	static void LogContent(LoggerType eType, const char* logType, const char* format, ...)
	{
		buff.clear();
#if !splitLogFileByType
		buff.append(logType);
		buff.append("--");
#endif
#ifdef LOG_TYPE_AS_TITLE
		switch (eType)
		{
		case LoggerType::LOGGER_LOG:
			buff.append("[Log]:");
			break;
		case LoggerType::LOGGER_WARN:
			buff.append("[Warning]:");
			break;
		case LoggerType::LOGGER_ERROR:
			buff.append("[Error]:");
			break;
		default:
			return;
		}
#endif
		memset(cbuffer, 0, sizeof(cbuffer));
		va_list valist;
		va_start(valist, format);
		vsprintf_s(cbuffer, format, valist);
		buff.append(cbuffer);
		buff.append("\n");
		va_end(valist);
		_logLock.lock();
#if !LOG_IN_THREAD
#if !splitLogFileByType
		printf(buff->c_str());
#else
		printf("%s--%s", logType, buff.c_str());
#endif
#if logToFile
		LogToFile(logType, buff.c_str());
#endif
#else
		logContents.insert(std::pair<std::string*, const char*>(new std::string(buff), logType));
#endif
		_logLock.unlock();
	}

private:
	GlobalLogger() {}
#if logToFile
	static void LogToFile(const char* logType, const char* content)
	{
		if (!InitType(logType))
			return;
		std::ofstream* fs = nullptr;
#if !splitLogFileByType
		fs = _logFiles[globalLogFilePath];
#else
		fs = _logFiles[logType];
#endif
		if (nullptr == fs || !fs->is_open())
			return;
		*fs << content;;
		fs->flush();
	}
	static bool InitType(const char* logType)
	{
#if !splitLogFileByType
		if (_logFiles.find(globalLogFilePath) == _logFiles.end())
		{
			FILE* f = nullptr;
			fopen_s(&f, globalLogFilePath, "w+");
			if (f != nullptr)
			{
				_logFiles.insert(std::pair<const char*, FILE*>(globalLogFilePath, f));
			}
			else
			{
				return false;
			}
		}
#else
		if (_logFiles.find(logType) == _logFiles.end())
		{
			char filePath[1250] = { 0 };
			boost::filesystem::path path = boost::filesystem::current_path();
			sprintf_s(filePath, "%s/%s%s.txt", path.string().c_str(), globalLogFilePath, logType);
			auto dir = boost::filesystem::path(filePath).parent_path();
			if (!boost::filesystem::exists(dir))
				boost::filesystem::create_directory(dir);
			//if (!boost::filesystem::exists(filePath))
			//	boost::filesystem::create_directory(filePath);
			std::ofstream* f = new std::ofstream(filePath, std::ios_base::in | std::ios_base::app);
			if (f != nullptr)
			{
				_logFiles.insert(std::pair<const char*, std::ofstream*>(logType, f));
			}
			else
			{
				return false;
			}
		}
		return true;
#endif
	}
#if LOG_IN_THREAD
	static void LogThreaded()
	{
		while (true)
		{
			sleep(10);
			_logLock.lock();
			if (logContents.size() > 0)
			{
				for (auto itr= logContents.rbegin();itr!= logContents.rend();itr++)
				{
					logContentsSwap.insert(*itr);
				}
				logContents.clear();
			}
			_logLock.unlock();
			for (auto itr : logContentsSwap)
			{
#if !splitLogFileByType
				printf(itr->c_str());
#else
				printf("%s--%s", itr.second, itr.first->c_str());
#endif
#if logToFile
				LogToFile(itr.second, itr.first->c_str());
#endif
				delete itr.first;
			}
			logContentsSwap.clear();
		}
	}
#endif
#endif
};
#endif