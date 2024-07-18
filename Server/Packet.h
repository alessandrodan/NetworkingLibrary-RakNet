#pragma once

#include <Network/PacketDefinition.h>

enum PacketCGHeader : Net::TPacketHeader
{
	HEADER_CG_AUTH_REQUEST = Net::HEADER_FIRST_AVAILABLE,
	HEADER_CG_HANDSHAKE,
	HEADER_CG_ACTION1,
};

enum PacketGCHeader : Net::TPacketHeader
{
	HEADER_GC_RESPONSE = Net::HEADER_FIRST_AVAILABLE,
	HEADER_GC_HANDSHAKE,
};

#pragma pack(push, 1)
typedef struct SPacketGCHandshake
{
	uint8_t	bHeader;
	uint32_t	dwHandshake;
	uint32_t	dwTime;
	long	lDelta;
} TPacketGCHandshake;

typedef struct SPacketCGHandshake
{
	uint8_t	bHeader;
	uint32_t	dwHandshake;
	uint32_t	dwTime;
	long	lDelta;
} TPacketCGHandshake;



struct TPacketCGAuthRequest
{
	PacketCGHeader header = PacketCGHeader::HEADER_CG_AUTH_REQUEST;
	char username[50];
	char password[50];
};

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
