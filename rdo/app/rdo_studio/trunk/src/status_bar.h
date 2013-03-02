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
#include "utils/warning_disable.h"
#include <QProgressBar>
#include <QMainWindow>
#include <QLabel>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include <utils/smart_ptr/intrusive_ptr.h>
// --------------------------------------------------------------------------------

OBJECT(StatusBar)
{
DECLARE_FACTORY(StatusBar)
public:
	enum Type
	{
		SB_COORD,
		SB_MODIFY,
		SB_OVERWRITE,
		SB_MODEL_TIME,
		SB_MODEL_RUNTYPE,
		SB_MODEL_SPEED,
		SB_MODEL_SHOWRATE
	};

	template <Type N>
	void update(CREF(QString) message)
	{
		update(StatusBarType<N>(), message);
	}

	void beginProgress(rsint lower, rsint upper);
	void stepProgress ();
	void endProgress  ();

private:
	StatusBar(QMainWindow* pParent);
	virtual ~StatusBar();

	QMainWindow*   m_pParent;
	QLabel*        m_pSBCoord;
	QLabel*        m_pSBModify;
	QLabel*        m_pSBOverwrite;
	QLabel*        m_pSBModelTime;
	QLabel*        m_pSBModelRuntype;
	QLabel*        m_pSBModelSpeed;
	QLabel*        m_pSBModelShowRate;
	QProgressBar*  m_pProgressBar;
	QWidget*       m_pProgressBarFakeWidget;

	template <Type N>
	struct StatusBarType: boost::mpl::integral_c<Type, N>
	{};

	template <Type N>
	void update(StatusBarType<N> statusBar, CREF(QString) message)
	{
		PTR(QLabel) pLabel = getLabel(statusBar);
		ASSERT(pLabel);
		pLabel->setText(message);
	}

	template <Type N>
	PTR(QLabel) getLabel(StatusBarType<N>);
};

#endif // _RDO_STUDIO_STATUS_BAR_H_
