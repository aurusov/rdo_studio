#ifndef _RDO_STUDIO_DOCK_DEBUG_H_
#define _RDO_STUDIO_DOCK_DEBUG_H_

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

#endif // _RDO_STUDIO_DOCK_DEBUG_H_
