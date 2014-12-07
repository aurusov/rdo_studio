#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QProgressBar>
#include <QMainWindow>
#include <QLabel>
#include <boost/mpl/integral_c.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

PREDECLARE_POINTER(StatusBar);
class StatusBar: public rdo::counter_reference
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
	void update(const QString& message)
	{
		update(StatusBarType<N>(), message);
	}

	void beginProgress(int lower, int upper);
	void stepProgress();
	void endProgress();

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
	void update(StatusBarType<N> statusBar, const QString& message)
	{
		QLabel* pLabel = getLabel(statusBar);
		ASSERT(pLabel);
		pLabel->setText(message);
	}

	template <Type N>
	QLabel* getLabel(StatusBarType<N>);
};
