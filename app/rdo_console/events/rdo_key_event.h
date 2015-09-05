#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

class KeyEvent: public Event
{
public:
    enum class State
    {
        PRESS = 0,
        RELEASE
    };

    KeyEvent(const std::string& name, Time time, State state, int key_code);
    virtual ~KeyEvent();

    void setState(State state);
    void setKeyCode(int keyCode);

    State getState() const;
    int  getKeyCode() const;

private:
    State state;
    int keyCode;
};

} // namespace rdo
