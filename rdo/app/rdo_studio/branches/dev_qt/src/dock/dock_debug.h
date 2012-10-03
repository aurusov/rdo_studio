/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_DEBUG_H_
#define _RDO_STUDIO_DOCK_DEBUG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/src/mfc_qt_wrapper.h"
// --------------------------------------------------------------------------------

class DockDebug: public QDockWidget
{
public:
	typedef  MFCQtWrapper<rdoEditCtrl::RDODebugEdit>  Context;

	DockDebug(PTR(QWidget) pParent);
	virtual ~DockDebug();

	void appendString(CREF(tstring) str);
	void clear();

	REF(Context::context_type) getContext();

private:
	typedef  QDockWidget  parent_type;

	PTR(Context)  m_pContext;
};

#endif // _RDO_STUDIO_DOCK_DEBUG_H_
