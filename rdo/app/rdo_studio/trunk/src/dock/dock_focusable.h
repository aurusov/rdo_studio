/*!
  \copyright (c) RDO-Team, 2013
  \file      dock_focusable.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_FOCUSABLE_H_
#define _RDO_STUDIO_DOCK_FOCUSABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class DockFocusable: public QDockWidget
{
Q_OBJECT
public:
	typedef  QDockWidget  super;

	DockFocusable(CREF(QString) title, PTR(QWidget) pParent);
	virtual ~DockFocusable();

private slots:
	void onVisibilityChanged(bool visible);
};

#endif // _RDO_STUDIO_DOCK_FOCUSABLE_H_
