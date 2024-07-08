#pragma once

#include <Network/AbstractPacketHeaderMap.h>

class CClientPacketHeaderMap : public Net::CAbstractPacketHeaderMap
{
	public:
		CClientPacketHeaderMap();
		~CClientPacketHeaderMap() = default;

	private:
		void __LoadPacketHeaders();
};
