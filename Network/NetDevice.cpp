#include "StdAfx.h"
#include "NetDevice.h"

namespace Net
{
	SLNet::RakPeerInterface* CNetDevice::peer = nullptr;

	void CNetDevice::Destroy()
	{
		peer->Shutdown(1000, 0, IMMEDIATE_PRIORITY);
		SLNet::RakPeerInterface::DestroyInstance(peer);
	}

	void CNetDevice::CloseConnection()
	{
		if (auto target = peer->GetSystemAddressFromIndex(0); target != SLNet::UNASSIGNED_SYSTEM_ADDRESS)
			peer->CloseConnection(target, true, 0, IMMEDIATE_PRIORITY);
	}

	bool CNetDevice::Create()
	{
		peer = SLNet::RakPeerInterface::GetInstance();
		return peer;
	}
}
