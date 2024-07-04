#pragma once

#include "slikenet/peerinterface.h"

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static bool Create();

		private:
			static SLNet::RakPeerInterface* peer;
	};
}
