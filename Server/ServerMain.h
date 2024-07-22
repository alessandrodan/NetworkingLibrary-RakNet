#pragma once

#include <Network/AbstractEntityServer.hpp>
#include <Network/AbstractPacketServerHandler.h>
#include "PeerManager.h"

class ServerMain : public Net::CAbstractEntityServer
{
	public:
		ServerMain() = default;
		~ServerMain() = default;

	protected:
		void __OnInitFail(int errorCode) override;
		void __OnInitSuccess() override;

	public:
		void ProcessNet() override;

	public:
		void DisconnectAll();
		void DisconnectFirstPeer();

	private:
		std::unique_ptr<CPeerManager> m_peerManager;
};
