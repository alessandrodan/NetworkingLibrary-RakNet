#include "StdAfx.h"
#include "PeerManager.h"
#include <iostream>
#include <Network/NetDevice.h>
#include <Network/Definition.h>

using namespace Net;

CPeerManager::CPeerManager()
{
    m_iPeerConnected = 0;
    m_iHandleCount = 0;

    m_map_handshake.clear();
    m_mapPeer.clear();
}

void CPeerManager::AcceptPeer(SLNet::RakNetGUID guid)
{
	if (m_iPeerConnected >= SERVER_MAX_INCOMING_CONNECTIONS)
	{
		std::cerr << "max connection reached. MAX_ALLOW_USER = %d" << SERVER_MAX_INCOMING_CONNECTIONS;
        CNetDevice::peer->CloseConnection(guid, true, 0, HIGH_PRIORITY);
		return;
	}

    const auto handshake = __CreateHandshake();
	auto newPeer = std::make_shared<CPeer>();
    newPeer->Setup(guid, ++m_iHandleCount, handshake);

    m_map_handshake.emplace(handshake, newPeer);
    m_mapPeer.emplace(guid, newPeer);

    ++m_iPeerConnected;
}

std::shared_ptr<CPeer> CPeerManager::GetPeer(SLNet::RakNetGUID guid)
{
    const auto it = m_mapPeer.find(guid);
    if (it == m_mapPeer.end())
        return nullptr;

    return it->second;
}

uint32_t thecore_random()
{
    return rand() % (1024 * 1024);
}

uint32_t get_global_time()
{
    return static_cast<uint32_t>(time(0));
}

uint32_t GetCRC32(const char* buf, size_t len)
{
    uint32_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc += static_cast<uint32_t>(buf[i]);
    }
    return crc;
}

uint32_t CPeerManager::__CreateHandshake()
{
    uint32_t crc;
    char crc_buf[8];

    std::map<uint32_t, std::shared_ptr<CPeer>>::iterator it;

    while (true)
    {
        do
        {
            uint32_t val = thecore_random() % (1024 * 1024);

            *(uint32_t*)(crc_buf) = val;
            *((uint32_t*)crc_buf + 1) = get_global_time();

            crc = GetCRC32(crc_buf, 8);
            it = m_map_handshake.find(crc);
        } while (it != m_map_handshake.end());

        if (crc != 0)
            break;
    }

    return crc;
}
