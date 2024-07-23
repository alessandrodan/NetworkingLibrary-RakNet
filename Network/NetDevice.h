#pragma once

#include "slikenet/peerinterface.h"
#include "Definition.h"

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static void CloseConnection();
			static bool Create();

			static Interface* peer;
	};
}
