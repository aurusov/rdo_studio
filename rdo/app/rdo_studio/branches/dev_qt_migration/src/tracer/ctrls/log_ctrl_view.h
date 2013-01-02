/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_ctrl_view.h
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_VIEW_H_
#define _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/log_ctrl.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogCtrlView
// --------------------------------------------------------------------------------
class LogCtrlView: public QAbstractScrollArea
{
public:
	LogCtrlView(PTR(QWidget) pParent);

	REF(LogCtrl) view()
	{
		return *static_cast<PTR(LogCtrl)>(viewport());
	}

private:
	virtual rbool viewportEvent(PTR(QEvent)      pEvent);
	virtual void  focusInEvent (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent(PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent(PTR(QKeyEvent)   pEvent);
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_VIEW_H_
