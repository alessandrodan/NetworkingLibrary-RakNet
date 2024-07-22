#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerHandshake : public Net::CAbstractPacketServerHandler<ServerHandshake>
{
	public:
		ServerHandshake();
		~ServerHandshake() = default;

	public:
		void LoadPacketHeaders() override;
		void ProcessPacketError(Net::EProcessPacketError errorType, SLNet::Packet* packet, Net::CAbstractPeer* peer) override;

	public:
		bool RecvHandshake(SLNet::Packet* packet, Net::CAbstractPeer* peer);
};
