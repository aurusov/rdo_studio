/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event_xml_parser.h
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
#include "utils/rdocommon.h"

#include "app/rdo_console/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class event_xml_parser
{
public:
    typedef std::list<boost::shared_ptr<event>> event_list;

public:
    bool register_parser(CREF(tstring) name, boost::shared_ptr<event_xml_reader> reader);

    void parse(REF(std::istream) stream, REF(event_list) list);

private:
    typedef std::map< tstring, boost::shared_ptr<event_xml_reader> > parsers;

    parsers m_parsers;
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_KEY_EVENT_XML_PARSER_H_