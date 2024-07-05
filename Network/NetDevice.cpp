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

	bool CNetDevice::Create()
	{
		peer = SLNet::RakPeerInterface::GetInstance();
		return peer;
	}
}
