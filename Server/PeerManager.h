#pragma once

#include "Peer.h"
#include <memory>
#include <map>

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_GAME,
	PHASE_AUTH,
};

class CPeerManager
{
	public:
		CPeerManager();
		~CPeerManager() = default;

		void AcceptPeer(SLNet::RakNetGUID guid);
		std::shared_ptr<CPeer> GetPeer(SLNet::RakNetGUID guid);

	protected:
		uint32_t __CreateHandshake();

	private:
		int				m_iPeerConnected;
		int				m_iHandleCount;
		std::map<uint32_t, std::shared_ptr<CPeer>> m_map_handshake;
		std::map<SLNet::RakNetGUID, std::shared_ptr<CPeer>> m_mapPeer;
};
