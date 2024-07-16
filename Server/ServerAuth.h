#pragma once

#include <Network/AbstractPacketHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerAuth : public Net::CAbstractPacketHandler
{
	public:
		ServerAuth();
		~ServerAuth() = default;

		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	private:
		using PacketManager = Net::CPacketManager<ServerAuth>;
		std::unique_ptr<PacketManager> m_packetHeader;

	public:
		bool TestRecv(SLNet::Packet* packet);

};
