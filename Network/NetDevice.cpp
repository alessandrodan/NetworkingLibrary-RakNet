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

	void CNetDevice::CloseConnection(const AddressOrGUID systemIdentifier, bool sendDisconnectionNotification)
	{
		if (CNetDevice::peer->GetConnectionState(systemIdentifier) == ConnectionState::IS_CONNECTED)
		{
			peer->CloseConnection(
				systemIdentifier,
				sendDisconnectionNotification,
				0,
				sendDisconnectionNotification ? HIGH_PRIORITY : LOW_PRIORITY
			);
		}
	}

	bool CNetDevice::IsConnectedToSystem(const AddressOrGUID systemIdentifier)
	{
		return CNetDevice::peer->GetConnectionState(systemIdentifier) == ConnectionState::IS_CONNECTED;
	}

	bool CNetDevice::Create()
	{
		peer = Interface::GetInstance();
		return peer;
	}
}
