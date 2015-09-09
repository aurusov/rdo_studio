// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_mouse_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

MouseEvent::MouseEvent(const std::string& name, Time time, Button button, int x, int y)
    : Event(name, time, Event::Type::MOUSE)
    , button(button)
    , x(x)
    , y(y)
{}

MouseEvent::~MouseEvent()
{}

void MouseEvent::setButton(Button button)
{
    this->button = button;
}

void MouseEvent::setX(int x)
{
    this->x = x;
}

void MouseEvent::sety(int y)
{
    this->y = y;
}

MouseEvent::Button MouseEvent::getButton() const
{
    return button;
}

int MouseEvent::getX() const
{
    return x;
}

int MouseEvent::getY() const
{
    return y;
}

} // namespace rdo
