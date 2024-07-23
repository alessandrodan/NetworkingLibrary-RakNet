#include "StdAfx.h"
#include "PeerManager.h"
#include <iostream>
#include <Network/NetDevice.h>
#include <vector>
#include "Packet.h"

using namespace Net;

CPeerManager::CPeerManager()
{
    m_iPeerConnected = 0;
    m_iHandleCount = 0;
    m_bDestroyed = false;

    m_setHandshake.clear();
    m_mapPeer.clear();
}

CPeerManager::~CPeerManager()
{
    Destroy();
}

void CPeerManager::Destroy()
{
    if (m_bDestroyed)
        return;

    m_bDestroyed = true;

    for (const auto& peer : m_mapPeer)
    {
        auto d = peer.second;
        DestroyDesc(d.get(), true);
    }

    m_mapPeer.clear();
    m_setHandshake.clear();
}

void CPeerManager::DestroyClosed()
{
    std::vector<uint32_t> toErase;
    for (const auto& peer : m_mapPeer)
    {
        auto d = peer.second;
        if (d->IsPhase(PHASE_CLOSE))
            toErase.push_back(peer.first);
    }

    for (const auto& guid : toErase)
    {
        auto it = m_mapPeer.find(guid);
        if (it != m_mapPeer.end())
            DestroyDesc(it->second.get());
    }
}

void CPeerManager::DestroyDesc(CPeer* d, bool skipMapErase)
{
    if (!d)
        return;

    if (!skipMapErase)
    {
        if (d->GetHandshake())
            m_setHandshake.erase(d->GetHandshake());

        if (d->GetGUID() != Net::UNASSIGNED_SYSTEM_GUID)
            m_mapPeer.erase(d->GetGUID().ToUint32(d->GetGUID()));
    }

    --m_iPeerConnected;
}

void CPeerManager::AcceptPeer(Net::GUID guid)
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

    m_setHandshake.emplace(handshake);
    m_mapPeer.emplace(guid.ToUint32(guid), newPeer);

    ++m_iPeerConnected;
}

std::shared_ptr<CPeer> CPeerManager::GetPeer(Net::GUID guid)
{
    const auto it = m_mapPeer.find(guid.ToUint32(guid));
    if (it == m_mapPeer.end())
        return nullptr;

    return it->second;
}

std::shared_ptr<CPeer> CPeerManager::GetFirstPeer()
{
    if (m_mapPeer.empty())
        return nullptr;

    return m_mapPeer.begin()->second;
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
    char crc_buf[8];

    auto generate_crc = [&]() -> uint32_t {
        uint32_t val = thecore_random() % (1024 * 1024);
        *(uint32_t*)(crc_buf) = val;
        *((uint32_t*)crc_buf + 1) = get_global_time();
        return GetCRC32(crc_buf, 8);
    };

    uint32_t crc = generate_crc();
    while (crc == 0 || m_setHandshake.find(crc) != m_setHandshake.end())
        crc = generate_crc();

    return crc;
}

CPeer* CPeerManager::ValidPeer(Net::CAbstractPeer* peer)
{
    CPeer* d = dynamic_cast<CPeer*>(peer);
    if (!d)
        return nullptr;

    return d;
}
