#pragma once

#include <cstdint>
#include "NetDevice.h"
#include "Definition.h"

namespace Net
{
	typedef SLNet::MessageID TPacketHeader;

	constexpr TPacketHeader HEADER_FIRST_AVAILABLE = ID_USER_PACKET_ENUM;

	struct NetPacket
	{
		Address systemAddress;
		GUID guid;
		unsigned char header;
		unsigned int length;
		unsigned char* data;
	};

	inline void ConvertToNetPacket(SLNet::Packet* packet, NetPacket* netPacket)
	{
		netPacket->systemAddress = packet->systemAddress;
		netPacket->guid = packet->guid;
		netPacket->header = packet->data[0];
		netPacket->length = packet->length;
		netPacket->data = packet->data;
	}

	inline SLNet::Packet* ReceivePacket()
	{
		return CNetDevice::Receive();
	}

	inline void DeallocatePacket(SLNet::Packet* packet)
	{
		CNetDevice::DeallocatePacket(packet);
	}
}
