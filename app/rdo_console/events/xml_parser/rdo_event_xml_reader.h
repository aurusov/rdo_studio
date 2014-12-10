#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/property_tree/ptree.hpp>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class Event;

class EventXmlReader
{
public:
    virtual ~EventXmlReader();

    virtual std::shared_ptr<Event> read(const boost::property_tree::ptree& pt) const = 0;
};

} // namespace rdo
