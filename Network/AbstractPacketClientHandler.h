#pragma once

#include "AbstractPacketHandler.h"

namespace Net
{
    class CAbstractPacketClientHandler : public CAbstractPacketHandler
    {
        public:
            CAbstractPacketClientHandler() = default;
            virtual ~CAbstractPacketClientHandler() = 0;

        public:
            virtual void Process() = 0;
    };

    inline CAbstractPacketClientHandler::~CAbstractPacketClientHandler() {}
}
