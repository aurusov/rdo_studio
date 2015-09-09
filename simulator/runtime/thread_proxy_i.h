#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

PREDECLARE_OBJECT_INTERFACE(IThreadProxy)
struct IThreadProxy: public rdo::RefCounter<IThreadProxy>
{
DECLARE_FACTORY(IThreadProxy)
public:
    enum class ThreadID
    {
        REPOSITORY
    };
    virtual void sendMessage(ThreadID threadID, std::size_t messageID, void* pParam) = 0;

protected:
    IThreadProxy()
    {}
    virtual ~IThreadProxy()
    {}
};
