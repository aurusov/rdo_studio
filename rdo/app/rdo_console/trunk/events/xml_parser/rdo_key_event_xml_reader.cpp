/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_key_event_xml_reader.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_key_event.h"
#include "app/rdo_console/events/xml_parser/rdo_key_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

key_event_xml_reader::~key_event_xml_reader()
{}

PTR(event) key_event_xml_reader::read(CREF(boost::property_tree::ptree) pt) const
{
	const tstring name = pt.get<tstring>("<xmlattr>.name", "");
	const double time  = pt.get<double>("<xmlattr>.time", 0.0);

	const boost::property_tree::ptree& param = pt.get_child("param");

	const int state = param.get<int>("<xmlattr>.state");
	const int key_code = param.get<int>("<xmlattr>.key");

	return new key_event(name, time, static_cast<key_event::states>(state), key_code);
}

} // namespace rdo
