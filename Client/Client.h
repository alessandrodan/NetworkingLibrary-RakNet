#pragma once

#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketClientHandler.h>
#include <Network/PacketManager.hpp>

class Client : public Net::CAbstractEntity, Net::CAbstractPacketClientHandler
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void ProcessNet() override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		bool IsConnected();

		void SendHandshake(uint32_t dwHandshake, uint32_t dwTime, long lDelta);
		void TestSend();
		bool RecvHandshake(SLNet::Packet* packet);
		bool TestRecv(SLNet::Packet* packet);

	private:
		bool isConnected;

		using PacketManager = Net::CPacketManagerClient<Client>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
