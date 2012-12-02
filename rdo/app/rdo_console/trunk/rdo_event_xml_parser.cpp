/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event_xml_parser.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/rdo_event.h"
#include "app/rdo_console/rdo_event_xml_parser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

bool event_xml_parser::register_parser(CREF(tstring) name, boost::shared_ptr<event_xml_reader> reader)
{
    if(m_parsers.find(name) == m_parsers.end())
    {
        m_parsers[name] = reader;
        return true;
    }
    return false;
}

void event_xml_parser::parse(REF(std::istream) stream, REF(event_list) list)
{
    list.clear();

    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(stream, pt);

    BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, pt.get_child("events") )
    {
        tstring event_type = v.second.get<tstring>("<xmlattr>.type", "");
        parsers::iterator it = m_parsers.find(event_type);
        if(it != m_parsers.end())
        {
            event* e = it->second->read(v);
            if(e != nullptr)
            {
                list.push_back(boost::shared_ptr<event>(e));
            }
            else
            {
                throw std::runtime_error("read event from XML");
            }
        }
        else
        {
            throw std::runtime_error("read unknown event type from XML");
        }
    }
}

CLOSE_RDO_NAMESPACE