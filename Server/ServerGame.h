#pragma once

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
		void ProcessPacketError(Net::EProcessPacketError errorType, Net::NetPacket* packet, Net::CAbstractPeer* peer) override;

	public:
		bool TestRecv(Net::NetPacket* packet, Net::CAbstractPeer* peer);
		bool TestSend(Net::NetPacket* packet, Net::CAbstractPeer* peer);
};
