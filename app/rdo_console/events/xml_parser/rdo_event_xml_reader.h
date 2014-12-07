#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/property_tree/ptree.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class event;

class event_xml_reader
{
public:
	virtual ~event_xml_reader();

	virtual event* read(const boost::property_tree::ptree& pt) const = 0;
};

} // namespace rdo
