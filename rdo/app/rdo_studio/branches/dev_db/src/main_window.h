/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_window.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MAIN_WINDOW_H_
#define _RDO_STUDIO_MAIN_WINDOW_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <math.h>
#include <boost/mpl/integral_c.hpp>
#include <QMainWindow>
#include <QSlider>
#include <QSignalMapper>
#include "ui_main_window.h"
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "app/rdo_studio/src/main_window_base.h"
#include "app/rdo_studio/src/status_bar.h"
// --------------------------------------------------------------------------------

class MainWindow
	: public QMainWindow
	, public MainWindowBase
	, public Ui::MainWindow
	, public ActionActivator
{
Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

	void init();

	virtual void updateAllStyles();

	virtual void setVisible(rbool visible);

	CREF(LPStatusBar) statusBar() const;

	void update_start();
	void update_stop ();

	virtual void addSubWindow              (QWidget* pWidget);
	virtual void activateSubWindow         (QWidget* pWidget);
	virtual void connectOnActivateSubWindow(QObject* pObject);

	PTR(QSlider) m_pModelSpeedSlider;

	void insertMenuFileReopenItem(CREF(QString) item);

	template <typename SlotFun>
	void updateInsertMenu(rbool enabled, const typename QtPrivate::FunctionPointer<SlotFun>::Object* pObject, SlotFun pSlot)
	{
		updateInsertMenu(enabled);
		void (QSignalMapper::*pSignal)(QObject*) = &QSignalMapper::mapped;
		if (enabled)
		{
			ASSERT(pObject);
			QObject::connect(m_pInsertMenuSignalMapper, pSignal, pObject, pSlot, Qt::UniqueConnection);
		}
		else
		{
			QObject::disconnect(m_pInsertMenuSignalMapper, pSignal, NULL, NULL);
		}
	}

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
	typedef  QMainWindow  parent_type;
	typedef  QStringList  ReopenList;
	typedef  std::map<QMdiSubWindow*, QAction*> SubWindows;

	SubWindows      m_pSubWindows;
	int             m_updateTimerID;
	LPStatusBar     m_pStatusBar;
	ReopenList      m_reopenList;
	QSignalMapper*  m_pInsertMenuSignalMapper;
	QAction*        m_pSeparator;
	QActionGroup*   m_pWindowAction;

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
	bool eventFilter(QObject *target, QEvent *event);

	void onSubWindowActivated(QMdiSubWindow*);
	void onViewOptions ();
	void onHelpWhatsNew();
	void onHelpAbout   ();

	void onDockVisibleChanged(bool visible);
	void onToolBarModelOrientationChanged(Qt::Orientation orientation);

	void onMenuFileReopen(QAction* pAction);

	void updateInsertMenu(rbool enabled);
	void onUpdateActions(rbool activated);
	void onUpdateTabMode(rbool activated);

	void addNewAction(QMdiSubWindow* window);
	void removeExcessActions();
	void addFirstSubWindow();
	void removeLastSubWindow();

	void setTabbedViewMode(bool checked);
};

#endif // _RDO_STUDIO_MAIN_WINDOW_H_
