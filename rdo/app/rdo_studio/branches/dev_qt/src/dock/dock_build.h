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
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/src/mfc_qt_wrapper.h"
// --------------------------------------------------------------------------------

class DockBuild: public QDockWidget
{
public:
	typedef  MFCQtWrapper<rdoEditCtrl::RDOBuildEdit>  Context;

	DockBuild(PTR(QWidget) pParent);
	virtual ~DockBuild();

	void appendString(CREF(tstring) str);
	void appendString(CREF(rdo::simulation::report::FileMessage) message);
	void clear();

	REF(Context::context_type) getContext();

private:
	typedef  QDockWidget  parent_type;

	PTR(Context)  m_pContext;
};

#endif // _RDO_STUDIO_DOCK_BUILD_H_
