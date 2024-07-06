#pragma once

#include <slikenet/types.h>

class Server
{
	public:
		Server() = default;
		~Server() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();

		bool TestRecv(SLNet::Packet* packet);
		bool TestSend(SLNet::Packet* packet);
};
