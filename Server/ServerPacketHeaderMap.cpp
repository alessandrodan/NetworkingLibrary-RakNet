#include "StdAfx.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketHeaderMap.h"

using namespace Net;

CServerPacketHeaderMap::CServerPacketHeaderMap()
{
    __LoadPacketHeaders();
}

void CServerPacketHeaderMap::__LoadPacketHeaders()
{
    Set(PacketCGHeader::HEADER_CG_ACTION1, CAbstractPacketHeaderMap::TPacketType(sizeof(TPacketCGAction1)));
}
