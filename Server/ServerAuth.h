#pragma once

#include <Network/AbstractEntity.h>
#include <Network/PacketManager.hpp>
#include <memory>

class ServerAuth : public Net::CAbstractEntity
{
	public:
		ServerAuth();
		~ServerAuth() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	private:
		using PacketManager = Net::CPacketManager<ServerAuth>;
		std::unique_ptr<PacketManager> m_packetHeader;

	public:
		bool TestRecv(SLNet::Packet* packet);

};
