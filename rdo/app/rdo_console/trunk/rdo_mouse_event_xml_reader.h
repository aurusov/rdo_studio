/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event_xml_reader.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_MOUSE_EVENT_XML_READER_H_
#define _RDO_MOUSE_EVENT_XML_READER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class mouse_event_xml_reader : public event_xml_reader
{
public:
    virtual ~mouse_event_xml_reader();

    virtual PTR(event) read(CREF(boost::property_tree::ptree::value_type) value);
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_MOUSE_EVENT_XML_READER_H_
