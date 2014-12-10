#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

#include <time.h>

namespace rdo {

class Event
{
public:
    enum class Type
    {
        NONE,
        KEY,
        MOUSE
    };

    typedef double Time;

public:
    Event(const std::string& name, Time time, Type type = Type::NONE);
    virtual ~Event();

    void setName(const std::string& name);
    void setTime(Time time);

    const std::string& getName() const;
    Time getTime() const;
    Type getType() const;

private:
    std::string name;
    Time time;
    Type type;
};

} // namespace rdo
