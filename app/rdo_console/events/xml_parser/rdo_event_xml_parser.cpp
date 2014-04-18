/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_event_xml_parser.cpp
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
#include "app/rdo_console/events/xml_parser/rdo_event_xml_parser.h"
// --------------------------------------------------------------------------------

namespace rdo {

bool event_xml_parser::register_parser(const std::string& name, boost::shared_ptr<event_xml_reader> reader)
{
	if (m_parsers.find(name) == m_parsers.end())
	{
		m_parsers[name] = reader;
		return true;
	}
	return false;
}

void event_xml_parser::parse(std::istream& stream, event_container& list) const
{
	list.clear();

	boost::property_tree::ptree pt;
	boost::property_tree::read_xml(stream, pt);

	BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, pt.get_child("rscript.events"))
	{
		const boost::property_tree::ptree& node = v.second;

		const std::string event_type = node.get<std::string>("<xmlattr>.type", "");
		parsers::const_iterator it = m_parsers.find(event_type);
		if (it != m_parsers.end())
		{
			event* e = it->second->read(node);
			if (e)
			{
				list.insert(std::make_pair(e->getTime(), boost::shared_ptr<event>(e)));
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

} // namespace rdo
