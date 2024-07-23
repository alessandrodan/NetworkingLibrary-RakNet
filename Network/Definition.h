#pragma once

#include <slikenet/types.h>
#include <slikenet/BitStream.h>

#define SERVER_MAX_CONNECTIONS	10
#define SERVER_MAX_INCOMING_CONNECTIONS	5
#define CLIENT_MAX_CONNECTIONS	1
#define CLIENT_CONNECTION_ATTEMPT_COUNT	10
#define CLIENT_TIME_BTW_SEND_CONNECTION_ATTEMPT_COUNT	1000

namespace Net
{
	using Address = SLNet::SystemAddress;
	constexpr Address UNASSIGNED_SYSTEM_ADDRESS = SLNet::UNASSIGNED_SYSTEM_ADDRESS;

	using GUID = SLNet::RakNetGUID;
	constexpr GUID UNASSIGNED_SYSTEM_GUID = SLNet::UNASSIGNED_RAKNET_GUID;

	using AddressOrGUID = SLNet::AddressOrGUID;

	using ConnectionState = SLNet::ConnectionState;

	using Socket = SLNet::SocketDescriptor;

	using StartResult = SLNet::StartupResult;

	using ConnectionAttemptResult = SLNet::ConnectionAttemptResult;

	using Interface = SLNet::RakPeerInterface;

	using Message = SLNet::BitStream;
}
