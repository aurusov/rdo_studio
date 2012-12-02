/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_mouse_event_xml_reader.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_mouse_event.h"
#include "app/rdo_console/rdo_mouse_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

mouse_event_xml_reader::~mouse_event_xml_reader()
{
}

PTR(event) mouse_event_xml_reader::read(CREF(boost::property_tree::ptree::value_type) value)
{
    tstring name = value.second.get<tstring>("<xmlattr>.name", "");;
    double time  = value.second.get<double>("time");
    int button = value.second.get<int>("button");
    int x = value.second.get<int>("x");
    int y = value.second.get<int>("y");

    return new mouse_event(name, time, static_cast<mouse_event::buttons>(button), x, y);
}

CLOSE_RDO_NAMESPACE
