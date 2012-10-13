/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_build.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_BUILD_H_
#define _RDO_STUDIO_DOCK_BUILD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
// --------------------------------------------------------------------------------

class DockBuild: public DockBase<rdoEditCtrl::RDOBuildEdit>
{
public:
	DockBuild(PTR(QWidget) pParent);
	virtual ~DockBuild();

	void appendString(CREF(tstring) str);
	void appendString(CREF(rdo::simulation::report::FileMessage) message);
	void clear();

private:
	typedef  DockBase<rdoEditCtrl::RDOBuildEdit>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_BUILD_H_
