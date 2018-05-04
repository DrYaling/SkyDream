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
#include <boost/filesystem.hpp>
#if WIN32
#include <windows.h>
#endif
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
static std::map<const char*,std::ofstream*> _logFiles = std::map<const char*, std::ofstream*>();
static std::mutex _logLock;
static std::string buff;
static char cbuffer[2048] = { 0 };
enum LoggerType
{
	LOGGER_LOG = 0,
	LOGGER_WARN = 1,
	LOGGER_ERROR = 2
};
#define LogFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_LOG, logType, format, __VA_ARGS__)
#define Log(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_LOG, logType, format)
#define LogWarningFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_WARN, logType, format, __VA_ARGS__)
#define LogWarning(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_WARN, logType, format)
#define LogErrorFormat(logType,format, ...) GlobalLogger::LogContent(LoggerType::LOGGER_ERROR, logType, format, __VA_ARGS__)
#define LogError(logType,format) GlobalLogger::LogContent(LoggerType::LOGGER_ERROR, logType, format)
class GlobalLogger
{
public:
	/*static void LogFormat(const char* logType, const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		LogContent(LoggerType::LOGGER_LOG, logType, format, valist);
		va_end(valist);
	}
	static void Log(const char* logType, const char* content)
	{
		LogContent(LoggerType::LOGGER_LOG, logType, content);
	}
	static void LogWarningFormat(const char* logType, const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		LogContent(LoggerType::LOGGER_WARN, logType, format, valist, 0);
		va_end(valist);
	}
	static void LogWarning(const char* logType, const char* content)
	{
		LogContent(LoggerType::LOGGER_WARN, logType, content);
	}
	static void LogErrorFormat(const char* logType, const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		LogContent(LoggerType::LOGGER_ERROR, logType, format, valist, 0);
		va_end(valist);
	}
	static void LogError(const char* logType, const char* content)
	{
		LogContent(LoggerType::LOGGER_ERROR, logType, content);
	}	
	*/
	static void Clear(const char* logType = nullptr)
	{
		if (nullptr != logType)
		{
			auto itr = _logFiles.find(logType);
			if (itr != _logFiles.end())
			{
				std::ofstream*f  = itr->second;
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
		_logLock.lock();
		buff.clear();
#if !splitLogFileByType
		buff.append(logType);
		buff.append("--");
#endif
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
		memset(cbuffer, 0, sizeof(cbuffer));
		va_list valist;
		va_start(valist, format);
		vsprintf_s(cbuffer, format, valist);
		buff.append(cbuffer);
		buff.append("\n");
		va_end(valist);
#if !splitLogFileByType
		printf(buff.c_str());
#else
		printf("%s--%s", logType, buff.c_str());
#endif
#if logToFile
		LogToFile(logType, buff.c_str());
#endif
		buff.clear();
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
			std::ofstream* f = new std::ofstream(filePath, std::ios_base::in| std::ios_base::app);
			if (f!= nullptr)
			{
				_logFiles.insert(std::pair<const char*, std::ofstream*>(logType, f));
			}
			else
			{
				return false;
			}
		}
#endif
	}
#endif
};
#endif