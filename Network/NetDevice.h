#pragma once

#include "Definition.h"

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static bool IsConnectedToSystem(const AddressOrGUID systemIdentifier);
			static void CloseConnection(const AddressOrGUID systemIdentifier, bool sendDisconnectionNotification);
			static bool Create();

			static Interface* peer;
	};
}
