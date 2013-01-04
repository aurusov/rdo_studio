/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      status_bar.h
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_STATUS_BAR_H_
#define _RDO_STUDIO_STATUS_BAR_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qprogressbar.h>
#include <QtGui/qmainwindow.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include <utils/smart_ptr/intrusive_ptr.h>
// --------------------------------------------------------------------------------

OBJECT(StatusBar)
{
DECLARE_FACTORY(StatusBar)
public:
	void beginProgress(rsint lower, rsint upper);
	void stepProgress ();
	void endProgress  ();

private:
	StatusBar(QMainWindow* pParent);
	virtual ~StatusBar();

	QMainWindow*   m_pParent;
	QProgressBar*  m_pProgressBar;
	QWidget*       m_pProgressBarFakeWidget;
};

#endif // _RDO_STUDIO_STATUS_BAR_H_
