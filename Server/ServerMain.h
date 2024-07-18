#pragma once

#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketHandler.h>
#include "PeerManager.h"

class ServerMain : public Net::CAbstractEntity, Net::CAbstractPacketHandler
{
	public:
		ServerMain() = default;
		~ServerMain() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;

	public:
		void DisconnectAll();
		void DisconnectFirstPeer();

	private:
		std::unique_ptr<CPeerManager> m_peerManager;
};
