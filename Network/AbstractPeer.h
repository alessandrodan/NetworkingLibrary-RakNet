#pragma once

#include <slikenet/types.h>

namespace Net
{
    class CAbstractPeer
    {
        public:
            CAbstractPeer() = default;
            virtual ~CAbstractPeer() = 0;

        public:
            virtual void ProcessRecv(SLNet::Packet* packet) = 0;
            virtual void Packet(const void* c_pvData, int iSize) = 0;
    };

    inline CAbstractPeer::~CAbstractPeer() {}
}
