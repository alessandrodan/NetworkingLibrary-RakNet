#include "StdAfx.h"
#include "Peer.h"
#include "PeerManager.h"

CPeer::CPeer()
{
	m_guid = SLNet::UNASSIGNED_RAKNET_GUID;
	m_dwHandshake = 0;
	m_dwHandle = 0;
	m_iPhase = PHASE_CLOSE;

	m_packetHandler = nullptr;
	m_packetHandlerServerHandshake = std::make_unique<ServerHandshake>();
	m_packetHandlerServerAuth = std::make_unique<ServerAuth>();
	m_packetHandlerServerGame = std::make_unique<ServerGame>();
}

void CPeer::Setup(SLNet::RakNetGUID guid, int handleCount, uint32_t handshake)
{
	m_guid = guid;
	m_dwHandle = handleCount;

	StartHandshake(handshake);

	SetPhase(PHASE_HANDSHAKE);
}

void CPeer::StartHandshake(uint32_t handshake)
{
	m_dwHandshake = handshake;
}

void CPeer::ProcessRecv()
{
	m_packetHandler->Process();
}

void CPeer::SetPhase(int phase)
{
	m_iPhase = phase;

	switch (m_iPhase)
	{
		case PHASE_CLOSE:
			m_packetHandler = nullptr;
			break;

		case PHASE_HANDSHAKE:
			m_packetHandler = m_packetHandlerServerHandshake.get();
			break;
	}
}
