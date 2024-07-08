#pragma once

#include <slikenet/types.h>
#include "ServerPacketHeaderMap.h"

class Server
{
	public:
		Server();
		~Server() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();

		bool TestRecv(SLNet::Packet* packet);
		bool TestSend(SLNet::Packet* packet);

	private:
		std::unique_ptr<CServerPacketHeaderMap> m_packetHeader;
};
