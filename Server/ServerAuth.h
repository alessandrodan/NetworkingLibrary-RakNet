#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerAuth : public Net::CAbstractPacketHandlerBase<ServerAuth>
{
	public:
		ServerAuth();
		~ServerAuth() = default;

	public:
		void LoadPacketHeaders() override;

	public:
		bool TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer);

};
