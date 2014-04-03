/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_mouse_event_xml_reader.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_mouse_event.h"
#include "app/rdo_console/events/xml_parser/rdo_mouse_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

mouse_event_xml_reader::~mouse_event_xml_reader()
{}

PTR(event) mouse_event_xml_reader::read(CREF(boost::property_tree::ptree) pt) const
{
	const tstring name = pt.get<tstring>("<xmlattr>.name", "");
	const double time  = pt.get<double>("<xmlattr>.time", 0.0);

	const boost::property_tree::ptree& param = pt.get_child("param");

	const int button = param.get<int>("<xmlattr>.button");
	const int x = param.get<int>("<xmlattr>.x");
	const int y = param.get<int>("<xmlattr>.y");

	return new mouse_event(name, time, static_cast<mouse_event::buttons>(button), x, y);
}

} // namespace rdo
