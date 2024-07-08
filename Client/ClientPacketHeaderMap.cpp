#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ClientPacketHeaderMap.h"

using namespace Net;

CClientPacketHeaderMap::CClientPacketHeaderMap()
{
	__LoadPacketHeaders();
}

void CClientPacketHeaderMap::__LoadPacketHeaders()
{
	Set(PacketGCHeader::HEADER_GC_RESPONSE, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketGCResponse)));
}
