#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

#include <time.h>

namespace rdo {

/// base event class
class event
{
public:
    enum types {
        none,
        key,
        mouse
    };

    typedef double etime;

public:
    event(const std::string& name, etime time, types type = none);
    virtual ~event();

    void setName(const std::string& name);
    void setTime(etime time);

    std::string getName() const;
    etime getTime() const;
    types getType() const;

private:
    std::string m_name;
    etime m_time;
    types m_type;
};

} // namespace rdo
