#pragma once

#include <slikenet/types.h>
#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>

class ServerGame : public Net::CAbstractPacketServerHandler<ServerGame>
{
	public:
		ServerGame();
		~ServerGame() = default;

	public:
		void LoadPacketHeaders() override;
		void ProcessPacketError(Net::EProcessPacketError errorType, SLNet::Packet* packet, Net::CAbstractPeer* peer) override;

	public:
		bool TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer);
		bool TestSend(SLNet::Packet* packet, Net::CAbstractPeer* peer);
};
