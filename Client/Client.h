#pragma once

#include "ClientPacketHeaderMap.h"

class Client
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();

		bool IsConnected();

		void TestSend();

	private:
		bool isConnected;
		std::unique_ptr<CClientPacketHeaderMap> m_packetHeader;
};
