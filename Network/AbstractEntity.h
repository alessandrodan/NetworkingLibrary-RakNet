#pragma once

namespace Net
{
    class CAbstractEntity
    {
        public:
            CAbstractEntity() = default;
            virtual ~CAbstractEntity() = 0;

        public:
            virtual bool Initialize(const char* c_szAddr, int port) = 0;
            virtual void ProcessNet() = 0;
    };

    inline CAbstractEntity::~CAbstractEntity() {}
}
