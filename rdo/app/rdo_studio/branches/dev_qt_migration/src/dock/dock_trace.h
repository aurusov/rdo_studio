/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_TRACE_H_
#define _RDO_STUDIO_DOCK_TRACE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"
// --------------------------------------------------------------------------------

class DockTrace: public DockBase<rdoTracerLog::RDOTracerLogCtrl>
{
public:
	DockTrace(PTR(QWidget) pParent);
	virtual ~DockTrace();

private:
	typedef  DockBase<rdoTracerLog::RDOTracerLogCtrl>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_TRACE_H_
