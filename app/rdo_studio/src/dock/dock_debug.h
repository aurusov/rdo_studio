#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/editor/debug_edit.h"
// --------------------------------------------------------------------------------

class DockDebug: public DockFocusable
{
public:
	typedef rdo::gui::editor::Debug context_type;

	DockDebug(QWidget* pParent);
	virtual ~DockDebug();

	void appendString(const QString& str);
	void clear();

	context_type& getContext();
};
