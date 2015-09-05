// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_mouse_event.h"
#include "app/rdo_console/events/xml_parser/rdo_mouse_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

MouseEventXmlReader::~MouseEventXmlReader()
{}

std::shared_ptr<Event> MouseEventXmlReader::read(const boost::property_tree::ptree& pt) const
{
    const auto name = pt.get<std::string>("<xmlattr>.name", "");
    const auto time = pt.get<Event::Time>("<xmlattr>.time", 0.0);

    const auto& param = pt.get_child("param");

    const auto button = param.get<int>("<xmlattr>.button");
    const auto x = param.get<int>("<xmlattr>.x");
    const auto y = param.get<int>("<xmlattr>.y");

    return std::make_shared<MouseEvent>(name, time, static_cast<MouseEvent::Button>(button), x, y);
}

} // namespace rdo
