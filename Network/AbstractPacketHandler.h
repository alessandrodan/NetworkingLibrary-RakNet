#pragma once

namespace Net
{
    class CAbstractPacketHandler
    {
        public:
            CAbstractPacketHandler() = default;
            virtual ~CAbstractPacketHandler() = 0;

        public:
            virtual void Process() = 0;

        protected:
            virtual void __LoadPacketHeaders() = 0;
    };

    inline CAbstractPacketHandler::~CAbstractPacketHandler() {}
}