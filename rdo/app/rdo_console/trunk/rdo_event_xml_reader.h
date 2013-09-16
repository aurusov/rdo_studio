/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event_xml_reader.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_EVENT_XML_READER_H_
#define _RDO_EVENT_XML_READER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/property_tree/ptree.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class event;

class event_xml_reader
{
public:
	virtual ~event_xml_reader();

	virtual PTR(event) read(CREF(boost::property_tree::ptree) pt) = 0;
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_EVENT_XML_READER_H_
