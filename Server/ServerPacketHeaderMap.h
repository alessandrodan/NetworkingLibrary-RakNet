#pragma once

#include <Network/AbstractPacketHeaderMap.h>

class CServerPacketHeaderMap : public Net::CAbstractPacketHeaderMap
{
	public:
		CServerPacketHeaderMap();
		~CServerPacketHeaderMap() = default;

	private:
		void __LoadPacketHeaders();
};
