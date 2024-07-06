#pragma once

#include "slikenet/peerinterface.h"

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static void CloseConnection();
			static bool Create();

			static SLNet::RakPeerInterface* peer;
	};
}
