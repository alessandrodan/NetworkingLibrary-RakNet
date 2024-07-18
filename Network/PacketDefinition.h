#pragma once

#include <cstdint>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/types.h>

namespace Net
{
	typedef SLNet::MessageID TPacketHeader;

	constexpr TPacketHeader HEADER_FIRST_AVAILABLE = ID_USER_PACKET_ENUM;
}
