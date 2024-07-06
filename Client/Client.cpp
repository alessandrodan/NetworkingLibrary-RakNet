#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.cpp>
#include "Client.h"
#include <Network/Definition.h>

using namespace Net;

Client::Client()
{
	isConnected = false;
}

bool Client::Initialize(const char* c_szAddr, int port)
{
	SLNet::SocketDescriptor socketDescriptor;
	const auto startupResult = CNetDevice::peer->Startup(CLIENT_MAX_CONNECTIONS, &socketDescriptor, 1);
	if (startupResult != SLNet::RAKNET_STARTED)
	{
		std::cout << "Failed to start client! Quitting - error code: "<< startupResult << std::endl;
		return false;
	}

	const auto connectionAttemptResult = CNetDevice::peer->Connect(c_szAddr, port, 0, 0, 0, 0, CLIENT_CONNECTION_ATTEMPT_COUNT, CLIENT_TIME_BTW_SEND_CONNECTION_ATTEMPT_COUNT);
	if (connectionAttemptResult != SLNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Failed to connect client! Quitting - error code: " << connectionAttemptResult << std::endl;
		return false;
	}

	CNetDevice::peer->SetOccasionalPing(true);

	std::cout << "socket connected to the server" << std::endl;

    return true;
}

void Client::Process()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				std::cout <<  "Our connection request has been accepted" << std::endl;
				isConnected = true;
				break;

			case ID_CONNECTION_ATTEMPT_FAILED:
				std::cout << "Our connection request has been FAILED" << std::endl;
				isConnected = false;
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "Server Remote Disconnected" << std::endl;
				break;

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				std::cout << "Server Disconnected" << std::endl;
				break;

			case ID_CONNECTION_LOST:
				std::cout << "Server Connection lost" << std::endl;
				break;

			case ID_REMOTE_CONNECTION_LOST:
				std::cout << "Server Connection Remote lost" << std::endl;
				break;

			default:
				std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
				CNetDevice::peer->CloseConnection(packet->systemAddress, true);
		}
	}
}

bool Client::IsConnected()
{
	return isConnected;
}
