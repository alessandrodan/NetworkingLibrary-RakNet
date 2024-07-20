#pragma once

#include "AbstractPacketHandler.h"
#include "AbstractPeer.h"
#include <slikenet/types.h>

namespace Net
{
    class CAbstractPacketHandler
    {
        public:
            CAbstractPacketHandler() = default;
            virtual ~CAbstractPacketHandler() = 0;

        public:
            virtual void LoadPacketHeaders() = 0;
    };

    class CAbstractPacketClientHandler : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketClientHandler() = default;
            virtual ~CAbstractPacketClientHandler() = 0;

        public:
            virtual void Process(SLNet::Packet* packet) = 0;
    };

    class CAbstractPacketServerHandler : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketServerHandler() = default;
            virtual ~CAbstractPacketServerHandler() = 0;

        public:
            virtual void Process(CAbstractPeer* peer, SLNet::Packet* packet) = 0;
    };

    inline CAbstractPacketHandler::~CAbstractPacketHandler() {}
    inline CAbstractPacketClientHandler::~CAbstractPacketClientHandler() {}
    inline CAbstractPacketServerHandler::~CAbstractPacketServerHandler() {}
}
