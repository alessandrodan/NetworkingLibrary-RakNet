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
		void ProcessPacketError(Net::EProcessPacketError errorType, Net::NetPacket* packet, Net::CAbstractPeer* peer) override;

	public:
		bool TestRecv(Net::NetPacket* packet, Net::CAbstractPeer* peer);

};
