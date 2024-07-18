#pragma once

#include <slikenet/types.h>
#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>

class ServerGame : Net::CAbstractPacketServerHandler
{
	public:
		ServerGame();
		~ServerGame() = default;

		void Process(Net::CAbstractPeer* peer, SLNet::Packet* packet) override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		bool TestRecv(SLNet::Packet* packet);
		bool TestSend(SLNet::Packet* packet);

	private:
		using PacketManager = Net::CPacketManager<ServerGame>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
