#pragma once

#include <slikenet/types.h>
#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>

class ServerGame : public Net::CAbstractPacketHandlerBase<ServerGame>
{
	public:
		ServerGame();
		~ServerGame() = default;

	public:
		void LoadPacketHeaders() override;

	public:
		bool TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer);
		bool TestSend(SLNet::Packet* packet);
};
