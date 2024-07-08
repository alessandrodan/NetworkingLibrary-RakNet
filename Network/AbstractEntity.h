#pragma once

#include "PacketDefinition.h"

namespace Net
{
    class CAbstractEntity
    {
        public:
            CAbstractEntity() = default;
            virtual ~CAbstractEntity() = 0;

            virtual bool Initialize(const char* c_szAddr, int port) = 0;
            virtual void Process() = 0;

        protected:
            virtual void __LoadPacketHeaders() = 0;
    };

    inline CAbstractEntity::~CAbstractEntity() {}
}
