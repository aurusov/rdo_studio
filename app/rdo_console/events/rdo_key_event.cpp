// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_key_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

KeyEvent::KeyEvent(const std::string& name, Time time, State state, int key_code)
    : Event(name, time, Event::Type::KEY)
    , state(state)
    , keyCode(key_code)
{}

KeyEvent::~KeyEvent()
{}

void KeyEvent::setState(State state)
{
    this->state = state;
}

void KeyEvent::setKeyCode(int keyCode)
{
    this->keyCode = keyCode;
}

KeyEvent::State KeyEvent::getState() const
{
    return state;
}

int KeyEvent::getKeyCode() const
{
    return keyCode;
}

} // namespace rdo
