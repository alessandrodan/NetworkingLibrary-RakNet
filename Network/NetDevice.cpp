#include "StdAfx.h"
#include "NetDevice.h"

namespace Net
{
	SLNet::RakPeerInterface* CNetDevice::peer = nullptr;

	void CNetDevice::Destroy()
	{
		SLNet::RakPeerInterface::DestroyInstance(peer);
	}

	bool CNetDevice::Create()
	{
		peer = SLNet::RakPeerInterface::GetInstance();
		return peer;
	}
}
