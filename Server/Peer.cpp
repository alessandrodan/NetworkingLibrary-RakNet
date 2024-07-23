#include "StdAfx.h"
#include "Peer.h"
#include "PeerManager.h"
#include <iostream>
#include <Network/NetDevice.h>
#include <Network/PacketIO.hpp>
#include "Packet.h"

void gettimeofday(struct timeval* t, struct timezone* dummy)
{
	uint32_t millisec = GetTickCount();

	t->tv_sec = (millisec / 1000);
	t->tv_usec = (millisec % 1000) * 1000;
}

static uint32_t get_boot_sec()
{
	static struct timeval tv_boot = { 0L, 0L };

	if (tv_boot.tv_sec == 0)
		gettimeofday(&tv_boot, NULL);

	return tv_boot.tv_sec;
}

uint32_t get_dword_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	tv.tv_sec -= get_boot_sec();
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}



CPeer::CPeer()
{
	m_guid = Net::UNASSIGNED_SYSTEM_GUID;
	m_dwHandshake = 0;
	m_dwHandle = 0;
	m_iPhase = PHASE_CLOSE;
	m_bDestroyed = false;

	m_packetHandler = nullptr;
	m_packetHandlerServerHandshake = std::make_unique<ServerHandshake>();
	m_packetHandlerServerAuth = std::make_unique<ServerAuth>();
	m_packetHandlerServerGame = std::make_unique<ServerGame>();
}

CPeer::~CPeer()
{
	if (m_bDestroyed)
		return;

	m_bDestroyed = true;

	if (m_guid != Net::UNASSIGNED_SYSTEM_GUID)
	{
		const auto connectionState = Net::CNetDevice::peer->GetConnectionState(m_guid);
		if (connectionState == Net::ConnectionState::IS_CONNECTED)
		{
			std::cout << "SYSTEM: closing socket. DESC #" << m_guid.ToString() << std::endl;
			Net::CNetDevice::peer->CloseConnection(m_guid, false);
		}

		m_guid = Net::UNASSIGNED_SYSTEM_GUID;
	}
}

void CPeer::Setup(Net::GUID guid, int handleCount, uint32_t handshake)
{
	m_guid = guid;
	m_dwHandle = handleCount;

	SetPhase(PHASE_HANDSHAKE);

	StartHandshake(handshake);
}

void CPeer::StartHandshake(uint32_t handshake)
{
	m_dwHandshake = handshake;
	SendHandshake(get_dword_time(), 0);
}

void CPeer::SendHandshake(uint32_t dwCurTime, long lNewDelta)
{
	TPacketGCHandshake pack;

	pack.bHeader = HEADER_GC_HANDSHAKE;
	pack.dwHandshake = m_dwHandshake;
	pack.dwTime = dwCurTime;
	pack.lDelta = lNewDelta;

	Packet(&pack, sizeof(TPacketGCHandshake));
}

void CPeer::Packet(const void* c_pvData, int iSize)
{
	assert(iSize > 0);

	if (m_iPhase == PHASE_CLOSE)
		return;

	if (m_guid == Net::UNASSIGNED_SYSTEM_GUID)
		return;

	Net::CPacketIO::SendPacket(c_pvData, iSize, m_guid);
}

bool CPeer::HandshakeProcess(uint32_t dwTime, long lDelta)
{
	// Simulation of the handshake process. not concerned now
	return true;
}

void CPeer::ProcessRecv(SLNet::Packet* packet)
{
	if (m_iPhase == PHASE_CLOSE)
		return;

	m_packetHandler->Process(this, packet);
}

void CPeer::SetPhase(int phase)
{
	m_iPhase = phase;

	TPacketGCPhase pack;
	pack.header = HEADER_GC_PHASE;
	pack.phase = phase;
	Packet(&pack, sizeof(TPacketGCPhase));

	switch (m_iPhase)
	{
		case PHASE_CLOSE:
			m_packetHandler = nullptr;
			break;

		case PHASE_HANDSHAKE:
			m_packetHandler = m_packetHandlerServerHandshake.get();
			break;

		case PHASE_AUTH:
			m_packetHandler = m_packetHandlerServerAuth.get();
			break;

		case PHASE_GAME:
			m_packetHandler = m_packetHandlerServerGame.get();
			break;
	}
}
