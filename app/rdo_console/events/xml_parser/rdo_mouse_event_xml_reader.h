#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/xml_parser/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class MouseEventXmlReader: public EventXmlReader
{
public:
    virtual ~MouseEventXmlReader();

    virtual std::shared_ptr<Event> read(const boost::property_tree::ptree& pt) const;
};

} // namespace rdo
