/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_frm.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MAIN_FRM_H_
#define _RDO_STUDIO_MFC_MAIN_FRM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
#include <boost/mpl/integral_c.hpp>
#include <QtGui/qmainwindow.h>
#include <QtGui/qlabel.h>
#include <QtGui/qslider.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/status_bar.h"
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_main_window.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioMainFrame
// --------------------------------------------------------------------------------
class RDOStudioMainFrame
	: public QMainWindow
	, public MainWindowBase
	, public Ui::MainWindow
{
Q_OBJECT

friend class RDOToolBar;

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

	enum StatusBar
	{
		SB_COORD,
		SB_MODIFY,
		SB_INSERTOVERWRITE,
		SB_MODEL_TIME,
		SB_MODEL_RUNTYPE,
		SB_MODEL_SPEED,
		SB_MODEL_SHOWRATE
	};

	void init();

	virtual void updateAllStyles();

	virtual void setVisible(rbool visible);

	CREF(LPStatusBar) statusBar() const;

	void update_start();
	void update_stop ();

	PTR(CWnd) c_wnd() { return &m_thisCWnd; }

	virtual void addSubWindow              (QWidget* pWidget);
	virtual void activateSubWindow         (QWidget* pWidget);
	virtual void connectOnActivateSubWindow(QObject* pObject);

	template <StatusBar N>
	void updateStatusBar(CREF(QString) message)
	{
		updateStatusBar(StatusBarType<N>(), message);
	}

	PTR(QSlider) m_pModelSpeedSlider;

private:
	typedef  QMainWindow  parent_type;

	CWnd                    m_thisCWnd;
	int                     m_updateTimerID;

	PTR(QLabel)             m_pSBCoord;
	PTR(QLabel)             m_pSBModify;
	PTR(QLabel)             m_pSBModelTime;
	PTR(QLabel)             m_pSBModelRuntype;
	PTR(QLabel)             m_pSBModelSpeed;
	PTR(QLabel)             m_pSBModelShowRate;
	LPStatusBar             m_pStatusBar;

	PTR(QToolBar)           m_pFileToolBar;
	PTR(QToolBar)           m_pEditToolBar;
	PTR(QToolBar)           m_pZoomToolBar;
	PTR(QToolBar)           m_pModelToolBar;

	void createStatusBar();
	void createToolBar  ();

	virtual void closeEvent(QCloseEvent* event);
	virtual void showEvent (QShowEvent*  event);
	virtual void hideEvent (QHideEvent*  event);
	virtual void timerEvent(QTimerEvent* event);

	template <StatusBar N>
	struct StatusBarType: boost::mpl::integral_c<StatusBar, N>
	{};

	template <StatusBar N>
	void updateStatusBar(StatusBarType<N> statusBar, CREF(QString) message)
	{
		PTR(QLabel) pLabel = getStatusBarLabel(statusBar);
		ASSERT(pLabel);
		pLabel->setText(message);
	}

	template <StatusBar N>
	PTR(QLabel) getStatusBarLabel(StatusBarType<N>);

private slots:
	void onViewOptions();
	void onHelpContext();
	void onHelpAbout  ();

	void onDockVisibleChanged(bool visible);
	void onModelToolBarOrientationChanged(Qt::Orientation orientation);

private:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMethodCommandRange( UINT id );
	afx_msg void OnMethodUpdateRange( CCmdUI* pCmdUI );
};

#endif // _RDO_STUDIO_MFC_MAIN_FRM_H_
