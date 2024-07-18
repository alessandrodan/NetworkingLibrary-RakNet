#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerHandshake : public Net::CAbstractPacketServerHandler
{
	public:
		ServerHandshake();
		~ServerHandshake() = default;

		void Process(Net::CAbstractPeer* peer, SLNet::Packet* packet) override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		bool RecvHandshake(SLNet::Packet* packet);

	private:
		using PacketManager = Net::CPacketManager<ServerHandshake>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
