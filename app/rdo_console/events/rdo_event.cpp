// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

Event::Event(const std::string& name, Time time, Type type)
    : name(name)
    , time(time)
    , type(type)
{}

Event::~Event()
{}

void Event::setName(const std::string& name)
{
    this->name = name;
}

void Event::setTime(double time)
{
    this->time = time;
}

const std::string& Event::getName() const
{
    return name;
}

Event::Time Event::getTime() const
{
    return time;
}

Event::Type Event::getType() const
{
    return type;
}

} // namespace rdo
