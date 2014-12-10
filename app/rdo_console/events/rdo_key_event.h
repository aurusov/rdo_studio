#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

class key_event: public event
{
public:
    enum class states
    {
        press = 0,
        release
    };

    key_event(const std::string& name, double time, states state, int key_code);
    virtual ~key_event();

    void setState(states state);
    void setKeyCode(int key_code);

    states getState() const;
    int  getKeyCode() const;

private:
    states m_state;
    int m_key_code;
};

} // namespace rdo
