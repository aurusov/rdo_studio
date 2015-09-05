#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
#include <iostream>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "app/rdo_console/events/rdo_event.h"
#include "app/rdo_console/events/xml_parser/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class EventXmlParser
{
public:
    typedef std::map<Event::Time, std::shared_ptr<Event>> Events;

public:
    bool registerParser(const std::string& name, std::shared_ptr<EventXmlReader> reader);

    void parse(std::istream& stream, Events& events) const;

private:
    typedef std::map<std::string, std::shared_ptr<EventXmlReader>> Parsers;

    Parsers parsers;
};

} // namespace rdo
