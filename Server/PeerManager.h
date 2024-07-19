#pragma once

#include "Peer.h"
#include <memory>
#include <set>
#include <unordered_map>

class CPeerManager
{
	public:
		CPeerManager();
		~CPeerManager();

		void Destroy();
		void DestroyClosed();
		void DestroyDesc(CPeer* d, bool skipMapErase = false);

		void AcceptPeer(SLNet::RakNetGUID guid);
		std::shared_ptr<CPeer> GetPeer(SLNet::RakNetGUID guid);
		std::shared_ptr<CPeer> GetFirstPeer();


	protected:
		uint32_t __CreateHandshake();

	private:
		int				m_iPeerConnected;
		int				m_iHandleCount;
		bool			m_bDestroyed;
		std::set<uint32_t> m_setHandshake;
		std::unordered_map<uint32_t, std::shared_ptr<CPeer>> m_mapPeer;
};
