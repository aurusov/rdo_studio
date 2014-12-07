#ifndef _RDO_STUDIO_DOCK_FOCUSABLE_H_
#define _RDO_STUDIO_DOCK_FOCUSABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDockWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class DockFocusable: public QDockWidget
{
Q_OBJECT
public:
	typedef  QDockWidget  super;

	DockFocusable(const QString& title, QWidget* pParent);
	virtual ~DockFocusable();

private slots:
	void onVisibilityChanged(bool visible);
};

#endif // _RDO_STUDIO_DOCK_FOCUSABLE_H_
