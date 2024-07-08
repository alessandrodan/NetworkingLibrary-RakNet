#pragma once

#include <Network/AbstractEntity.h>
#include <Network/PacketManager.hpp>

class Client : public Net::CAbstractEntity
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		bool IsConnected();

		void TestSend();
		bool TestRecv(SLNet::Packet* packet);

	private:
		bool isConnected;

		using PacketManager = Net::CPacketManager<Client>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
