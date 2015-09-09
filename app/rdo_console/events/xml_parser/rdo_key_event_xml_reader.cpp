// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_key_event.h"
#include "app/rdo_console/events/xml_parser/rdo_key_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

KeyEventXmlReader::~KeyEventXmlReader()
{}

std::shared_ptr<Event> KeyEventXmlReader::read(const boost::property_tree::ptree& pt) const
{
    const auto name = pt.get<std::string>("<xmlattr>.name", "");
    const auto time = pt.get<Event::Time>("<xmlattr>.time", 0.0);

    const auto& param = pt.get_child("param");

    const auto state = param.get<int>("<xmlattr>.state");
    const auto key_code = param.get<int>("<xmlattr>.key");

    return std::make_shared<KeyEvent>(name, time, static_cast<KeyEvent::State>(state), key_code);
}

} // namespace rdo
