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
    };

    inline CAbstractPeer::~CAbstractPeer() {}
}
