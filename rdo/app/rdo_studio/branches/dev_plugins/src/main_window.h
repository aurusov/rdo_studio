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
#include "utils/src/common/warning_disable.h"
#include <math.h>
#include <boost/mpl/integral_c.hpp>
#include <QMainWindow>
#include <QSlider>
#include <QSignalMapper>
#include "ui_main_window.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "app/rdo_studio/src/main_window_base.h"
#include "app/rdo_studio/src/status_bar.h"
// --------------------------------------------------------------------------------

class MainWindow
	: public QMainWindow
	, public MainWindowBase
	, public Ui::MainWindow
{
Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

	void init();

	virtual void updateAllStyles();

	virtual void setVisible(bool visible);

	CREF(LPStatusBar) statusBar() const;

	void update_start();
	void update_stop ();

	virtual void addSubWindow     (QWidget* pWidget);
	virtual void activateSubWindow(QWidget* pWidget);

	PTR(QSlider) m_pModelSpeedSlider;

	void insertMenuFileReopenItem(CREF(QString) item);

	template <typename SlotFun>
	void updateInsertMenu(bool enabled, const typename QtPrivate::FunctionPointer<SlotFun>::Object* pObject, SlotFun pSlot)
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

	struct SubWindowToAction
	{
		typedef  std::map<QMdiSubWindow*, QAction*>  Map;

		SubWindowToAction(MainWindow* pMainWindow);

		void onSubWindowActivated(QMdiSubWindow* pSubWindow);
		void onTitleChanged      (QMdiSubWindow* pSubWindow);

	private:
		MainWindow*   m_pMainWindow;
		Map           m_map;
		QAction*      m_pSeparator;
		QActionGroup* m_pActionGroup;

		Map::const_iterator addNewSubWindow(QMdiSubWindow* pSubWindow);
		void updateList();
		void addFirstSubWindow();
		void removeLastSubWindow();
	};

	std::auto_ptr<SubWindowToAction>  m_subWindowToAction;
	int                m_updateTimerID;
	LPStatusBar        m_pStatusBar;
	ReopenList         m_reopenList;
	QSignalMapper*     m_pInsertMenuSignalMapper;

	void createStatusBar ();
	void createToolBar   ();
	void createInsertMenu();

	void updateMenuFileReopen();
	void loadMenuFileReopen  ();
	void saveMenuFileReopen  () const;

	virtual void closeEvent (QCloseEvent* event);
	virtual void showEvent  (QShowEvent*  event);
	virtual void hideEvent  (QHideEvent*  event);
	virtual void timerEvent (QTimerEvent* event);
	virtual bool eventFilter(QObject* target, QEvent* event);

	void onViewOptions ();
	void onHelpWhatsNew();
	void onHelpAbout   ();

	void onDockVisibleChanged(bool visible);
	void onToolBarModelOrientationChanged(Qt::Orientation orientation);

	void onMenuFileReopen(QAction* pAction);
	void updateInsertMenu(bool enabled);

	void onUpdateCascadeTitle   (bool activated);
	void onUpdateTabMode        (bool activated);
	void onSetTabbedViewMode    (bool checked);
	void onSubWindowActivated   (QMdiSubWindow* window);
	void onSubWindowStateChanged(Qt::WindowStates oldState, Qt::WindowStates newState);
	template <class F>
	void forAllSubWindows(F functor, QMdiSubWindow* pTopSubWindow);

	void updateWindowTitle();
};

#endif // _RDO_STUDIO_MAIN_WINDOW_H_
