#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerAuth : public Net::CAbstractPacketServerHandler
{
	public:
		ServerAuth();
		~ServerAuth() = default;

		void Process(Net::CAbstractPeer* peer, SLNet::Packet* packet) override;

	protected:
		void __LoadPacketHeaders() override;

	private:
		using PacketManager = Net::CPacketManager<ServerAuth>;
		std::unique_ptr<PacketManager> m_packetHeader;

	public:
		bool TestRecv(SLNet::Packet* packet);

};
