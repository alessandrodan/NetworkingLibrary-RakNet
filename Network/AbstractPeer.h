#pragma once

#include "PacketDefinition.h"

namespace Net
{
    class CAbstractPeer
    {
        public:
            CAbstractPeer() = default;
            virtual ~CAbstractPeer() = 0;

        public:
            virtual void ProcessRecv(NetPacket* packet) = 0;
            virtual void Packet(const void* c_pvData, int iSize) = 0;
            virtual void SetPhase(int phase) = 0;
    };

    inline CAbstractPeer::~CAbstractPeer() {}
}
