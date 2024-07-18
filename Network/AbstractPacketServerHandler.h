#pragma once

#include "AbstractPacketHandler.h"
#include "AbstractPeer.h"
#include <slikenet/types.h>

namespace Net
{
    class CAbstractPacketServerHandler : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketServerHandler() = default;
            virtual ~CAbstractPacketServerHandler() = 0;

        public:
            virtual void Process(CAbstractPeer* peer, SLNet::Packet* packet) = 0;
    };

    inline CAbstractPacketServerHandler::~CAbstractPacketServerHandler() {}
}
