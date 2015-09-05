#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

class MouseEvent: public Event
{
public:
    enum class Button
    {
        NONE,
        LEFT,
        RIGHT,
        MIDDLE
    };

    MouseEvent(const std::string& name, Time time, Button button, int x, int y);
    virtual ~MouseEvent();

    void setButton(Button button);
    void setX(int x);
    void sety(int y);

    Button getButton() const;
    int getX() const;
    int getY() const;

private:
    Button button;
    int x;
    int y;
};

} // namespace rdo
