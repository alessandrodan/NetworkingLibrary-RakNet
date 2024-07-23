#pragma once

#include "AbstractPeer.h"

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

    class CAbstractPacketClientHandlerBase : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketClientHandlerBase() = default;
            virtual ~CAbstractPacketClientHandlerBase() = 0;

        public:
            virtual void Process(NetPacket* packet) = 0;
    };

    class CAbstractPacketServerHandlerBase : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketServerHandlerBase() = default;
            virtual ~CAbstractPacketServerHandlerBase() = 0;

        public:
            virtual void Process(CAbstractPeer* peer, NetPacket* packet) = 0;
    };

    inline CAbstractPacketHandler::~CAbstractPacketHandler() {}
    inline CAbstractPacketClientHandlerBase::~CAbstractPacketClientHandlerBase() {}
    inline CAbstractPacketServerHandlerBase::~CAbstractPacketServerHandlerBase() {}
}
