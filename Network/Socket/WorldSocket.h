#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include "Socket.h"
#include "Session.h"
#include "Common/MessageBuffer.h"
#include "Common/MPSCQueue.h"
#include <chrono>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;
struct EncryptablePacket
{
	char* buff;
	size_t size;
	uint32 opcode;
};
uint32 const SizeOfClientHeader = sizeof(uint32) + sizeof(uint16);
uint32 const SizeOfServerHeader = sizeof(uint32) + sizeof(uint16);

class WorldSocket : public Socket<WorldSocket>
{
	typedef Socket<WorldSocket> BaseSocket;

public:
	WorldSocket(tcp::socket* socket);

	WorldSocket(WorldSocket const& right) = delete;
	WorldSocket& operator=(WorldSocket const& right) = delete;

	void Start() override;
	bool Update() override;

	void SendPacket(const char*  packet, size_t size);

	void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }

protected:
	void OnClose() override;
	void ReadHandler() override;
	bool ReadHeaderHandler();

	enum class ReadDataHandlerResult
	{
		Ok = 0,
		Error = 1,
		WaitingForQuery = 2
	};

	ReadDataHandlerResult ReadDataHandler();
	void WritePacketToBuffer(EncryptablePacket const& packet, MessageBuffer& buffer);

private:

	uint32 _authSeed;

	std::chrono::steady_clock::time_point _LastPingTime;
	uint32 _OverSpeedPings;

	std::mutex _worldSessionLock;
	Session* _session;
	bool _authed;

	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;
	MPSCQueue<EncryptablePacket> _bufferQueue;
	//QueryCallbackProcessor _queryProcessor;

	std::string _ipCountry;
};

#endif
