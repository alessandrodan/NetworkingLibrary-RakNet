#pragma once

#include <slikenet/types.h>
#include <Network/AbstractEntity.h>
#include <Network/PacketManager.hpp>

class ServerGame : public Net::CAbstractEntity
{
	public:
		ServerGame();
		~ServerGame() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		bool TestRecv(SLNet::Packet* packet);
		bool TestSend(SLNet::Packet* packet);

	private:
		using PacketManager = Net::CPacketManager<ServerGame>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
