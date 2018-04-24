#include "UdpSocketClient.h"
#include "ClientSocketMgr.h"
ReadDataHandlerResult UdpSocketClient::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	std::cout << "UdpSocketClient read remote cmd " << header->Command << ",data size " << header->Size << std::endl;
	switch (header->Command)
	{
	case S2C_Opcode::S2C_PUNCH_COMFIRM:
	{
		SkyDream::Person remote;
		remote.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		sClientSocketMgr->client->StartC2CConnection(remote.ip().c_str(), remote.port());
		break;
	}
	case C2C_Opcode::C2C_HAND:
	{
		std::cout << "receive hand pack" << std::endl;
		break;
	}
	case C2C_Opcode::C2C_PING:
	{
		break;
	}
	case C2C_Opcode::C2C_MSG:
	{
		break;
	}
	default:
	{
		break;
	}
	}

	return ReadDataHandlerResult::Ok;
}