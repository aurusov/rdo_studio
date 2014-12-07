#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/tracer/logger/tracer_logger_main_wnd.h"
// --------------------------------------------------------------------------------

class DockTrace: public DockFocusable
{
public:
	typedef  rdo::gui::tracer::LogMainWnd  context_type;

	DockTrace(QWidget* pParent);
	virtual ~DockTrace();

	context_type& getContext();
};
