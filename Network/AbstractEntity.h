#pragma once

namespace Net
{
    class CAbstractEntity
    {
        public:
            CAbstractEntity() = default;
            virtual ~CAbstractEntity() = 0;

        public:
            virtual bool Start(const char* c_szAddr, int port) = 0;
            virtual void ProcessNet() = 0;

        protected:
            virtual bool __Initialize(const char* c_szAddr, int port) = 0;
            virtual void __OnInitFail(int errorCode) = 0;
            virtual void __OnInitSuccess() = 0;
    };

    inline CAbstractEntity::~CAbstractEntity() {}
}
