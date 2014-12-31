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
    enum class Message
    {
        BEFORE_DELETE = 0
    };

    void connect(INotify* pTo, Message message);
    void disconnect(INotify* pTo);
    void disconnect(INotify* pTo, Message message);
    void fireMessage(Message message, void* pParam) const;

private:
    typedef std::multimap<Message, INotify*> Connected;
    Connected m_connected;

    void disconnect(Connected::iterator it, INotify* pTo);
};

CLOSE_RDO_RUNTIME_NAMESPACE
