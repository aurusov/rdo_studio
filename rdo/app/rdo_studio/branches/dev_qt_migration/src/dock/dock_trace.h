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
#include "app/rdo_studio_mfc/src/dock/dock_focusable.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/log_main_wnd.h"
// --------------------------------------------------------------------------------

class DockTrace: public DockFocusable
{
public:
	typedef  rdo::gui::tracer::LogMainWnd  context_type;

	DockTrace(PTR(QWidget) pParent);
	virtual ~DockTrace();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_TRACE_H_
