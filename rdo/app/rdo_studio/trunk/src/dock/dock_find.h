/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_find.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_FIND_H_
#define _RDO_STUDIO_DOCK_FIND_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/edit_ctrls/rdofindedit.h"
// --------------------------------------------------------------------------------

class DockFind: public DockFocusable
{
public:
	typedef rdoEditCtrl::RDOFindEdit context_type;

	DockFind(PTR(QWidget) pParent);
	virtual ~DockFind();

	void appendString(
		CREF(tstring)                str,
		rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT,
		int lineNumber = -1, int posInLine = 0);
	void clear();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_FIND_H_
