#pragma once

#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketServerHandler.h>
#include "PeerManager.h"

class ServerMain : public Net::CAbstractEntity
{
	public:
		ServerMain() = default;
		~ServerMain() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void ProcessNet() override;

	public:
		void DisconnectAll();
		void DisconnectFirstPeer();

	private:
		std::unique_ptr<CPeerManager> m_peerManager;
};
