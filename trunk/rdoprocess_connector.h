#ifndef RDO_PROCESS_CONNECTOR_H
#define RDO_PROCESS_CONNECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object_chart.h"

// ----------------------------------------------------------------------------
// ---------- RPConnector
// ----------------------------------------------------------------------------
class RPConnectorDock;

class RPConnector: public RPObjectChart
{
public:
	RPConnector( RPObject* parent, const rp::string& name = "connector" );
	virtual ~RPConnector();

	RPConnectorDock* dock_begin;
	RPConnectorDock* dock_end;
};

#endif // RDO_PROCESS_CONNECTOR_H
