// ----------------------------------------------------------------------- INCLUDES
#include <stdexcept>
#include <boost/property_tree/xml_parser.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/xml_parser/rdo_event_xml_parser.h"
// --------------------------------------------------------------------------------

namespace rdo {

bool EventXmlParser::registerParser(const std::string& name, std::shared_ptr<EventXmlReader> reader)
{
    return parsers.insert(std::make_pair(name, reader)).second;
}

void EventXmlParser::parse(std::istream& stream, Events& events) const
{
    events.clear();

    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(stream, pt);

    for (const boost::property_tree::ptree::value_type& v: pt.get_child("rscript.events"))
    {
        const auto& node = v.second;

        const auto event_type = node.get<std::string>("<xmlattr>.type", "");
        Parsers::const_iterator it = parsers.find(event_type);
        if (it == parsers.end())
            throw std::runtime_error("read unknown event type from XML");

        const auto& event = it->second->read(node);
        if (!event)
            throw std::runtime_error("read event from XML");

        events.insert(std::make_pair(event->getTime(), event));
    }
}

} // namespace rdo
