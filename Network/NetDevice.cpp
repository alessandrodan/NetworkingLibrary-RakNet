#include "StdAfx.h"
#include "NetDevice.h"

namespace Net
{
	Interface* CNetDevice::peer = nullptr;

	void CNetDevice::Destroy()
	{
		peer->Shutdown(1000, 0, IMMEDIATE_PRIORITY);
		Interface::DestroyInstance(peer);
	}

	void CNetDevice::CloseConnection()
	{
		if (auto target = peer->GetSystemAddressFromIndex(0); target != Net::UNASSIGNED_SYSTEM_ADDRESS)
			peer->CloseConnection(target, true, 0, IMMEDIATE_PRIORITY);
	}

	bool CNetDevice::Create()
	{
		peer = Interface::GetInstance();
		return peer;
	}
}
