/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_key_event_xml_reader.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_key_event.h"
#include "app/rdo_console/rdo_key_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

key_event_xml_reader::~key_event_xml_reader()
{
}

PTR(event) key_event_xml_reader::read(CREF(boost::property_tree::ptree) pt)
{
	tstring name = pt.get<tstring>("<xmlattr>.name", "");
	double time  = pt.get<double>("<xmlattr>.time", 0.0);

	boost::property_tree::ptree const& param = pt.get_child("param");

	int state = param.get<int>("<xmlattr>.state");
	int key_code = param.get<int>("<xmlattr>.key");

	return new key_event(name, time, static_cast<key_event::states>(state), key_code);
}

CLOSE_RDO_NAMESPACE
