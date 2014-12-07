#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAbstractScrollArea>
#include "utils/src/common/warning_enable.h"
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

	LogView& view()
	{
		return *static_cast<LogView*>(viewport());
	}

private:
	virtual bool viewportEvent(QEvent*      pEvent);
	virtual void focusInEvent (QFocusEvent* pEvent);
	virtual void focusOutEvent(QFocusEvent* pEvent);
	virtual void keyPressEvent(QKeyEvent*   pEvent);
};

}}} // namespace rdo::gui::tracer
