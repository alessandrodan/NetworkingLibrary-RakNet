#pragma once

#include <slikenet/types.h>
#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketHandler.h>
#include <Network/PacketManager.hpp>

class ServerGame : Net::CAbstractPacketHandler
{
	public:
		ServerGame();
		~ServerGame() = default;

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
