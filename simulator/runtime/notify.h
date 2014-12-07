#pragma once

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/notify_i.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class Notify
{
public:
    enum Type
    {
        RO_BEFOREDELETE = 0
    };

    void connect(INotify* pTo, std::size_t message);
    void disconnect(INotify* pTo);
    void disconnect(INotify* pTo, std::size_t message);
    void fireMessage(std::size_t message, void* pParam) const;

private:
    typedef std::multimap<std::size_t, INotify*> Connected;
    Connected m_connected;

    void disconnect(Connected::iterator it, INotify* pTo);
};

CLOSE_RDO_RUNTIME_NAMESPACE
