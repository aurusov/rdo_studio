/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_event_xml_parser.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_KEY_EVENT_XML_PARSER_H_
#define _RDO_KEY_EVENT_XML_PARSER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
#include <iostream>

#include <boost/shared_ptr.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "app/rdo_console/rdo_event.h"
#include "app/rdo_console/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class event_xml_parser
{
public:
	typedef std::map< event::etime, boost::shared_ptr<event> > event_container;

public:
	bool register_parser(CREF(tstring) name, boost::shared_ptr<event_xml_reader> reader);

	void parse(REF(std::istream) stream, REF(event_container) list) const;

private:
	typedef std::map< tstring, boost::shared_ptr<event_xml_reader> > parsers;

	parsers m_parsers;
};

} // namespace rdo

#endif // _RDO_KEY_EVENT_XML_PARSER_H_
