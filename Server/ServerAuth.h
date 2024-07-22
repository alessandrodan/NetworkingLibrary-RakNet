#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerAuth : public Net::CAbstractPacketServerHandler<ServerAuth>
{
	public:
		ServerAuth();
		~ServerAuth() = default;

	public:
		void LoadPacketHeaders() override;
		void ProcessPacketError(Net::EProcessPacketError errorType, SLNet::Packet* packet, Net::CAbstractPeer* peer) override;

	public:
		bool TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer);

};
