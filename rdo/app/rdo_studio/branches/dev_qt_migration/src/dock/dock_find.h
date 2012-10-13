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
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
// --------------------------------------------------------------------------------

class DockFind: public DockBase<rdoEditCtrl::RDOFindEdit>
{
public:
	DockFind(PTR(QWidget) pParent);
	virtual ~DockFind();

	void appendString(
		CREF(tstring)                str,
		rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT,
		int lineNumber = -1, int posInLine = 0);
	void clear();

private:
	typedef  DockBase<rdoEditCtrl::RDOFindEdit>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_FIND_H_
