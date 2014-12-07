#ifndef _RDO_STUDIO_DOCK_FIND_H_
#define _RDO_STUDIO_DOCK_FIND_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/editor/find_edit.h"
// --------------------------------------------------------------------------------

class DockFind: public DockFocusable
{
public:
	typedef rdo::gui::editor::Find context_type;

	DockFind(QWidget* pParent);
	virtual ~DockFind();

	void appendString(
		const QString& str,
		rdo::model::FileType fileType = rdo::model::PAT,
		int lineNumber = -1, int posInLine = 0);
	void clear();

	context_type& getContext();
};

#endif // _RDO_STUDIO_DOCK_FIND_H_
