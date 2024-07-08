#pragma once

#include <cstdint>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/types.h>

namespace Net
{
	typedef SLNet::MessageID TPacketHeader;

	enum PacketCGHeader : TPacketHeader
	{
		HEADER_CG_ACTION1 = ID_USER_PACKET_ENUM,
	};

	enum PacketGCHeader : TPacketHeader
	{
		HEADER_GC_RESPONSE = ID_USER_PACKET_ENUM,
	};

#pragma pack(push, 1)
	struct TPacketCGAction1
	{
		PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION1;
		uint32_t numIntero;
	};

	struct TPacketGCResponse
	{
		PacketGCHeader header = PacketGCHeader::HEADER_GC_RESPONSE;
	};
#pragma pack(pop)
}
