/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_main_wnd.h
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_LOGGER_MAIN_WND_H_
#define _RDO_STUDIO_TRACER_LOGGER_MAIN_WND_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QAbstractScrollArea>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogMainWnd
// --------------------------------------------------------------------------------
class LogMainWnd: public QAbstractScrollArea
{
public:
	LogMainWnd(QWidget* pParent);

	REF(LogView) view()
	{
		return *static_cast<PTR(LogView)>(viewport());
	}

private:
	virtual rbool viewportEvent(PTR(QEvent)      pEvent);
	virtual void  focusInEvent (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent(PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent(PTR(QKeyEvent)   pEvent);
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_LOGGER_MAIN_WND_H_
