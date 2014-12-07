#ifndef _RDO_MOUSE_EVENT_XML_READER_H_
#define _RDO_MOUSE_EVENT_XML_READER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/xml_parser/rdo_event_xml_reader.h"
// --------------------------------------------------------------------------------

namespace rdo {

class mouse_event_xml_reader: public event_xml_reader
{
public:
	virtual ~mouse_event_xml_reader();

	virtual event* read(const boost::property_tree::ptree& pt) const;
};

} // namespace rdo

#endif // _RDO_MOUSE_EVENT_XML_READER_H_
