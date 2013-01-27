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
#include <QtGui/qslider.h>
#include <QtCore/qsignalmapper.h>
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

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

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

	PTR(QSlider) m_pModelSpeedSlider;

	void insertMenuFileReopenItem(CREF(tstring) item);

	void updateInsertMenu(rbool enabled, QObject* pObject, CREF(tstring) method);

	class InsertMenuData: public QObject
	{
	public:
		typedef  boost::optional<ruint>  Position;

		InsertMenuData(QObject* pParent, const QString& text, const Position& position = Position());

		const QString&  text    () const;
		const Position& position() const;

	private:
		QString   m_text;
		Position  m_position;
	};

private:
	typedef  QMainWindow           parent_type;
	typedef  std::vector<tstring>  ReopenList;

	CWnd            m_thisCWnd;
	int             m_updateTimerID;
	LPStatusBar     m_pStatusBar;
	ReopenList      m_reopenList;
	QSignalMapper*  m_pInsertMenuSignalMapper;

	void createStatusBar ();
	void createToolBar   ();
	void createInsertMenu();

	void updateMenuFileReopen();
	void loadMenuFileReopen  ();
	void saveMenuFileReopen  () const;

	virtual void closeEvent(QCloseEvent* event);
	virtual void showEvent (QShowEvent*  event);
	virtual void hideEvent (QHideEvent*  event);
	virtual void timerEvent(QTimerEvent* event);

private slots:
	void onViewOptions ();
	void onHelpWhatsNew();
	void onHelpAbout   ();

	void onDockVisibleChanged(bool visible);
	void onToolBarModelOrientationChanged(Qt::Orientation orientation);

	void onMenuFileReopen(QAction* pAction);
};

#endif // _RDO_STUDIO_MFC_MAIN_FRM_H_
