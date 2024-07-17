#pragma once

#include <Network/AbstractPacketHandler.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerHandshake : public Net::CAbstractPacketHandler
{
	public:
		ServerHandshake();
		~ServerHandshake() = default;

		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	private:
		using PacketManager = Net::CPacketManager<ServerHandshake>;
		std::unique_ptr<PacketManager> m_packetHeader;
};
