/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_view.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_
#define _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl.h"
// --------------------------------------------------------------------------------

namespace rdoTracerLog
{

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
class RDOTracerLogCtrl: public QAbstractScrollArea
{
public:
	RDOTracerLogCtrl(PTR(QWidget) pParent);

	REF(RDOLogCtrl) view()
	{
		return *static_cast<PTR(RDOLogCtrl)>(viewport());
	}

private:
	virtual rbool viewportEvent(PTR(QEvent)      pEvent);
	virtual void  focusInEvent (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent(PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent(PTR(QKeyEvent)   pEvent);
};

} // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_
