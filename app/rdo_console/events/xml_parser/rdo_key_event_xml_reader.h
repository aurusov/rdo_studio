#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/xml_parser/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class key_event_xml_reader: public event_xml_reader
{
public:
	virtual ~key_event_xml_reader();

	virtual event* read(const boost::property_tree::ptree& pt) const;
};

} // namespace rdo
