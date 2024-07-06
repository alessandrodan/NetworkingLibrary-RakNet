#pragma once

#include <cstdint>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/types.h>

namespace Net
{
	typedef SLNet::MessageID TPacketHeader;

	enum PacketHeader : TPacketHeader
	{
		HEADER_ACTION1 = ID_USER_PACKET_ENUM,
		HEADER_RESPONSE,
	};

#pragma pack(push, 1)
	struct TPacketAction1
	{
		PacketHeader header = PacketHeader::HEADER_ACTION1;
		uint32_t numIntero;
	};

	struct TPacketResponse
	{
		PacketHeader header = PacketHeader::HEADER_RESPONSE;
	};
#pragma pack(pop)
}
