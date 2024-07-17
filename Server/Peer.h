#pragma once

#include <slikenet/types.h>
#include <Network/AbstractPacketHandler.h>
#include "ServerHandshake.h"
#include "ServerAuth.h"
#include "ServerGame.h"

class CPeer
{
	public:
		CPeer();
		~CPeer() = default;

		void Setup(SLNet::RakNetGUID guid, int handleCount, uint32_t handshake);
		void StartHandshake(uint32_t handshake);

		void SetPhase(int phase);

		void ProcessRecv();

	private:
		SLNet::RakNetGUID m_guid;
		int			m_dwHandle;
		uint32_t			m_dwHandshake;
		int				m_iPhase;

		Net::CAbstractPacketHandler* m_packetHandler;
		std::unique_ptr<ServerHandshake> m_packetHandlerServerHandshake;
		std::unique_ptr<ServerAuth> m_packetHandlerServerAuth;
		std::unique_ptr<ServerGame> m_packetHandlerServerGame;
};
