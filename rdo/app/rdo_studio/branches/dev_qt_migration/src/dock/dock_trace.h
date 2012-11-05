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
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"
// --------------------------------------------------------------------------------

class DockTrace: public QDockWidget
{
public:
	typedef  rdoTracerLog::RDOTracerLogCtrl  context_type;

	DockTrace(PTR(QWidget) pParent);
	virtual ~DockTrace();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_TRACE_H_
