#pragma once

#include <Network/AbstractPacketServerHandler.h>
#include "ServerHandshake.h"
#include "ServerAuth.h"
#include "ServerGame.h"

class CPeer : public Net::CAbstractPeer
{
	public:
		CPeer();
		~CPeer();

	public:
		void ProcessRecv(SLNet::Packet* packet) override;
		void Packet(const void* c_pvData, int iSize) override;
		void SetPhase(int phase) override;

		void Setup(SLNet::RakNetGUID guid, int handleCount, uint32_t handshake);
		void StartHandshake(uint32_t handshake);

		void SendHandshake(uint32_t dwCurTime, long lNewDelta);
		bool HandshakeProcess(uint32_t dwTime, long lDelta);


		uint32_t GetHandshake() const { return m_dwHandshake; }
		SLNet::RakNetGUID GetGUID() const { return m_guid; }
		bool			IsPhase(int phase) const { return m_iPhase == phase ? true : false; }

	private:
		SLNet::RakNetGUID m_guid;
		int			m_dwHandle;
		uint32_t			m_dwHandshake;
		int				m_iPhase;
		bool			m_bDestroyed;

		Net::CAbstractPacketServerHandlerBase* m_packetHandler;
		std::unique_ptr<ServerHandshake> m_packetHandlerServerHandshake;
		std::unique_ptr<ServerAuth> m_packetHandlerServerAuth;
		std::unique_ptr<ServerGame> m_packetHandlerServerGame;
};
