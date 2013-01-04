/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_frm.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#include <boost/bind.hpp>
#include <QtCore/qprocess.h>
#include <QtGui/qmdisubwindow.h>
#include <QtGui/qtoolbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/help_context_i.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/options.h"
#include "app/rdo_studio_mfc/src/about.h"
#include "app/rdo_studio_mfc/src/view_preferences.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "thirdparty/sci/include/Scintilla.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioMainFrame
// --------------------------------------------------------------------------------
//! @todo qt
//BEGIN_MESSAGE_MAP(RDOStudioMainFrame, CMDIFrameWnd)
//	ON_WM_CREATE()
//	ON_WM_DESTROY()
//	ON_WM_TIMER()
//	ON_WM_CLOSE()
//	ON_WM_SHOWWINDOW()
//	ON_WM_SIZE()
//	ON_WM_ENTERMENULOOP()
//	ON_WM_EXITMENULOOP()
//	ON_WM_ENTERIDLE()
//END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORD_STATUSBAR,
	ID_MODIFY_STATUSBAR,
	ID_INSERTOVERWRITE_STATUSBAR,
	ID_MODEL_TIME_STATUSBAR,
	ID_MODEL_RUNTYPE_STATUSBAR,
	ID_MODEL_SPEED_STATUSBAR,
	ID_MODEL_SHOWRATE_STATUSBAR,
	ID_PROGRESSSTATUSBAR
};

rbool MainWindowBase::close_mode = false;

RDOStudioMainFrame::RDOStudioMainFrame()
	: m_updateTimerID(0)
{
	setupUi(this);
	mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);

	createStatusBar();
	createToolBar  ();

	connect(actViewSettings, SIGNAL(triggered(bool)), this, SLOT(onViewOptions()));
	connect(actHelpContext,  SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));
	connect(actHelpAbout,    SIGNAL(triggered(bool)), this, SLOT(onHelpAbout  ()));

	connect(m_pModelToolBar, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onModelToolBarOrientationChanged(Qt::Orientation)));

	Scintilla_LinkLexers();
}

//! todo не вызывается диструктор
RDOStudioMainFrame::~RDOStudioMainFrame()
{}

void RDOStudioMainFrame::createStatusBar()
{
	m_pSBCoord         = new QLabel(this);
	m_pSBModify        = new QLabel(this);
	m_pSBModelTime     = new QLabel(this);
	m_pSBModelRuntype  = new QLabel(this);
	m_pSBModelSpeed    = new QLabel(this);
	m_pSBModelShowRate = new QLabel(this);

	parent_type::statusBar()->addWidget(m_pSBCoord,         5);
	parent_type::statusBar()->addWidget(m_pSBModify,        5);
	parent_type::statusBar()->addWidget(m_pSBModelTime,     5);
	parent_type::statusBar()->addWidget(m_pSBModelRuntype,  7);
	parent_type::statusBar()->addWidget(m_pSBModelSpeed,    5);
	parent_type::statusBar()->addWidget(m_pSBModelShowRate, 7);

	m_pStatusBar = rdo::Factory<::StatusBar>::create(this);
}

void RDOStudioMainFrame::createToolBar()
{
	QSize iconSize(16, 15);

	m_pFileToolBar = addToolBar("Файл");
	m_pFileToolBar->setIconSize(iconSize);
	m_pFileToolBar->addAction(actFileNew);
	m_pFileToolBar->addAction(actFileOpen);
	m_pFileToolBar->addAction(actFileSave);
	m_pFileToolBar->addAction(actFileSaveAll);

	m_pEditToolBar = addToolBar("Редактирование");
	m_pEditToolBar->setIconSize(iconSize);
	m_pEditToolBar->addAction(actEditCut);
	m_pEditToolBar->addAction(actEditCopy);
	m_pEditToolBar->addAction(actEditPaste);
	m_pEditToolBar->addSeparator();
	m_pEditToolBar->addAction(actEditUndo);
	m_pEditToolBar->addAction(actEditRedo);
	m_pEditToolBar->addSeparator();
	m_pEditToolBar->addAction(actSearchFind);
	m_pEditToolBar->addAction(actSearchReplace);
	m_pEditToolBar->addSeparator();
	m_pEditToolBar->addAction(actSearchBookmarkPrev);
	m_pEditToolBar->addAction(actSearchBookmarkNext);

	m_pZoomToolBar = addToolBar("Масштаб");
	m_pZoomToolBar->setIconSize(iconSize);
	m_pZoomToolBar->addAction(actViewZoomInc);
	m_pZoomToolBar->addAction(actViewZoomDec);
	m_pZoomToolBar->addAction(actViewZoomAuto);
	m_pZoomToolBar->addAction(actViewZoomReset);

	m_pModelSpeedSlider = new QSlider(Qt::Horizontal, this);
	m_pModelSpeedSlider->setRange(0, 100);
	m_pModelSpeedSlider->setValue(100);
	m_pModelSpeedSlider->setMaximumSize(100, 100);

	m_pModelToolBar = addToolBar("Модель");
	m_pModelToolBar->setIconSize(iconSize);
	m_pModelToolBar->addAction(actModelRun);
	m_pModelToolBar->addAction(actModelStop);
	m_pModelToolBar->addSeparator();
	m_pModelToolBar->addAction(actModelRuntimeMaxSpeed);
	m_pModelToolBar->addAction(actModelRuntimeJump);
	m_pModelToolBar->addAction(actModelRuntimeSync);
	m_pModelToolBar->addAction(actModelRuntimePause);
	m_pModelToolBar->addSeparator();
	m_pModelToolBar->addAction(actModelShowRateInc);
	m_pModelToolBar->addAction(actModelShowRateIncFour);
	m_pModelToolBar->addAction(actModelShowRateDecFour);
	m_pModelToolBar->addAction(actModelShowRateDec);
	m_pModelToolBar->addSeparator();
	m_pModelToolBar->addWidget(m_pModelSpeedSlider);
	m_pModelToolBar->addSeparator();
	m_pModelToolBar->addAction(actModelFrameNext);
	m_pModelToolBar->addAction(actModelFramePrev);
}

void RDOStudioMainFrame::init()
{
	m_thisCWnd.Attach(winId());

	// Кто-то должен поднять кернел и треды
	new RDOStudioModel();

	style_editor.init( "editor" );
	style_editor.load();

	style_build.init( "build" );
	style_build.window->wordWrap          = true;
	style_build.window->showHorzScrollBar = false;
	style_build.load();

	style_debug.init( "debug" );
	style_debug.window->wordWrap          = true;
	style_debug.window->showHorzScrollBar = false;
	style_debug.load();

	style_trace.init( "trace" );
	style_trace.load();

	style_results.init( "results" );
	style_results.load();

	style_find.init( "find" );
	style_find.load();

	style_frame.init( "frame" );
	style_frame.load();

	style_chart.init( "chart" );
	style_chart.load();

	m_pDockBuild   = new DockBuild  (this);
	m_pDockDebug   = new DockDebug  (this);
	m_pDockTrace   = new DockTrace  (this);
	m_pDockResults = new DockResults(this);
	m_pDockFind    = new DockFind   (this);

	QObject::connect(m_pDockBuild, SIGNAL(visibilityChanged(bool)), this, SLOT(onDockVisibleChanged(bool)));
	QObject::connect(m_pDockFind,  SIGNAL(visibilityChanged(bool)), this, SLOT(onDockVisibleChanged(bool)));

	updateAllStyles();
	addDockWidget(Qt::BottomDockWidgetArea, m_pDockBuild);
	tabifyDockWidget(m_pDockBuild, m_pDockDebug  );
	tabifyDockWidget(m_pDockBuild, m_pDockTrace  );
	tabifyDockWidget(m_pDockBuild, m_pDockResults);
	tabifyDockWidget(m_pDockBuild, m_pDockFind   );
	m_pDockDebug->raise();

	m_pDockChartTree = new DockChartTree(this);
	m_pDockFrame     = new DockFrame    (this);
	addDockWidget(Qt::LeftDockWidgetArea, m_pDockChartTree);
	tabifyDockWidget(m_pDockChartTree, m_pDockFrame);
	m_pDockChartTree->raise();

	PTR(QMenu) pMenuDockView = new QMenu("Окна");
	ASSERT(pMenuDockView);
	menuView->insertMenu(actViewSettings, pMenuDockView);
	pMenuDockView->addAction(m_pDockBuild->toggleViewAction());
	pMenuDockView->addAction(m_pDockDebug->toggleViewAction());
	pMenuDockView->addAction(m_pDockTrace->toggleViewAction());
	pMenuDockView->addAction(m_pDockResults->toggleViewAction());
	pMenuDockView->addAction(m_pDockFind->toggleViewAction());
	pMenuDockView->addAction(m_pDockChartTree->toggleViewAction());
	pMenuDockView->addAction(m_pDockFrame->toggleViewAction());

	PTR(QMenu) pMenuToolbarView = new QMenu("Панели");
	ASSERT(pMenuToolbarView);
	menuView->insertMenu(actViewSettings, pMenuToolbarView);
	pMenuToolbarView->addAction(m_pFileToolBar->toggleViewAction());
	pMenuToolbarView->addAction(m_pEditToolBar->toggleViewAction());
	pMenuToolbarView->addAction(m_pZoomToolBar->toggleViewAction());
	pMenuToolbarView->addAction(m_pModelToolBar->toggleViewAction());

	menuView->insertSeparator(actViewSettings);

#ifdef PROCGUI_ENABLE
	m_pDockProcess = new DockProcess(this);
	tabifyDockWidget(m_pDockChartTree, m_pDockProcess);
	pMenuDockView->addAction(m_pDockProcess->toggleViewAction());
#else
	m_pDockProcess = NULL;
#endif

	//! @todo qt
	//! Создание popup-menu для редакторов из докод
	//popupMenu.CreatePopupMenu();

	//if (AfxGetMainWnd())
	//{
	//	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	//	if (mainMenu)
	//	{
	//		rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//		int delta = maximized ? 1 : 0;

	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 8, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	//		popupMenu.AppendMenu( MF_SEPARATOR );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );
	//		popupMenu.AppendMenu( MF_SEPARATOR );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 7, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 8, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 9, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 10, &popupMenu );
	//	}
	//}
	//build->setPopupMenu( &popupMenu );
	//debug->setPopupMenu( &popupMenu );
	//results->setPopupMenu( &popupMenu );
	//find->setPopupMenu( &popupMenu );

	g_pTracer->registerClipboardFormat();

	PTR(IInit) pModelInit = dynamic_cast<PTR(IInit)>(model);
	ASSERT(pModelInit);
	pModelInit->init();
}

void RDOStudioMainFrame::setVisible(rbool visible)
{
	parent_type::setVisible(visible);
}

void RDOStudioMainFrame::closeEvent(QCloseEvent* event)
{
	close_mode = true;
	if (model && !model->saveModified())
	{
		event->ignore();
	}
	close_mode = false;

	if (event->isAccepted())
	{
		update_stop();
		style_editor.save();
		style_build.save();
		style_debug.save();
		style_trace.save();
		style_results.save();
		style_find.save();
		style_frame.save();
		style_chart.save();
	}
}

void RDOStudioMainFrame::onViewOptions()
{
	ViewPreferences dlg(this);

	studioApp.getEditorEditStyle()->attachSubscriber(
		boost::bind(&ViewPreferences::onUpdateStyleNotify, &dlg, _1)
	);

	dlg.exec();

	studioApp.getEditorEditStyle()->detachSubscriber(
		boost::bind(&ViewPreferences::onUpdateStyleNotify, &dlg, _1)
	);
}

void RDOStudioMainFrame::updateAllStyles()
{
	model->updateStyleOfAllModel();
	getDockBuild  ().getContext().setEditorStyle (&style_build  );
	getDockDebug  ().getContext().setEditorStyle (&style_debug  );
	getDockTrace  ().getContext().view().setStyle(&style_trace  );
	getDockResults().getContext().setEditorStyle (&style_results);
	getDockFind   ().getContext().setEditorStyle (&style_find   );
	g_pTracer->updateChartsStyles();
}

void RDOStudioMainFrame::beginProgress(rsint lower, rsint upper)
{
	m_pStatusBar->beginProgress(lower, upper);
}

void RDOStudioMainFrame::stepProgress()
{
	m_pStatusBar->stepProgress();
};

void RDOStudioMainFrame::endProgress()
{
	m_pStatusBar->endProgress();
}

void RDOStudioMainFrame::onHelpContext()
{
	PTR(IHelpContext) pHelpContext = dynamic_cast<PTR(IHelpContext)>(focusWidget());
	if (pHelpContext)
	{
		pHelpContext->onHelpContext();
	}
	else
	{
		QByteArray ba;
		ba.append("setSource qthelp://language/doc/rdo_studio_rus/html/rdo_whats_new.htm\n");
		studioApp.callQtAssistant(ba);
	}
}

void RDOStudioMainFrame::onHelpAbout()
{
	About dlg(this);
	dlg.exec();
}

void RDOStudioMainFrame::update_start()
{
	m_updateTimerID = startTimer(1000 / 30);
}

void RDOStudioMainFrame::update_stop()
{
	if (m_updateTimerID)
	{
		killTimer(m_updateTimerID);
		m_updateTimerID = 0;
	}
}

void RDOStudioMainFrame::timerEvent(QTimerEvent* event)
{
	parent_type::timerEvent(event);

	if (event->timerId() == m_updateTimerID)
	{
		update_stop();
		model->update();
		update_start();
	}
}

void RDOStudioMainFrame::showEvent(QShowEvent*)
{}

void RDOStudioMainFrame::hideEvent(QHideEvent*)
{}

//! @todo qt
//void RDOStudioMainFrame::OnEnterMenuLoop( BOOL bIsTrackPopupMenu )
//{
//	model->setGUIPause();
//}
//
//void RDOStudioMainFrame::OnExitMenuLoop( BOOL bIsTrackPopupMenu )
//{
//	model->setGUIContinue();
//}

void RDOStudioMainFrame::addSubWindow(QWidget* pWidget)
{
	ASSERT(pWidget);

	QList<QMdiSubWindow*> frameList = mdiArea->subWindowList();

	rbool maximized = frameList.empty();
	if (!maximized)
	{
		maximized = frameList.front()->isMaximized();
	}

	QMdiSubWindow* pFrame = mdiArea->addSubWindow(pWidget);

	IInit* pInitWidget = dynamic_cast<IInit*>(pWidget);
	if (pInitWidget)
	{
		pInitWidget->init();
	}
	pWidget->show();

	static const float sizeScale = 0.9f;
	QSize size(mdiArea->size());
	size.setWidth (float(size.width ()) * sizeScale);
	size.setHeight(float(size.height()) * sizeScale);
	pFrame->move  (frameList.size() * 4, frameList.size() * 4);
	pFrame->resize(size);

	if (maximized)
	{
		pFrame->showMaximized();
	}
}

void RDOStudioMainFrame::activateSubWindow(QWidget* pWidget)
{
	QMdiSubWindow* pSubWindow = dynamic_cast<QMdiSubWindow*>(pWidget);
	if (!pSubWindow)
		return;

	mdiArea->setActiveSubWindow(pSubWindow);
}

void RDOStudioMainFrame::connectOnActivateSubWindow(QObject* pObject)
{
	ASSERT(mdiArea);
	ASSERT(pObject);

	QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), pObject, SLOT(onSubWindowActivated(QMdiSubWindow*)));
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_COORD>(StatusBarType<SB_COORD>)
{
	return m_pSBCoord;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODIFY>(StatusBarType<SB_MODIFY>)
{
	return m_pSBModify;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODEL_TIME>(StatusBarType<SB_MODEL_TIME>)
{
	return m_pSBModelTime;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODEL_RUNTYPE>(StatusBarType<SB_MODEL_RUNTYPE>)
{
	return m_pSBModelRuntype;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODEL_SPEED>(StatusBarType<SB_MODEL_SPEED>)
{
	return m_pSBModelSpeed;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODEL_SHOWRATE>(StatusBarType<SB_MODEL_SHOWRATE>)
{
	return m_pSBModelShowRate;
}

void RDOStudioMainFrame::onDockVisibleChanged(rbool visible)
{
	if (!visible)
		return;

	QDockWidget* pDock = dynamic_cast<QDockWidget*>(sender());
	ASSERT(pDock);

	rdoEditCtrl::LogEdit* pLog = NULL;

	if (pDock == &getDockBuild())
	{
		pLog = &getDockBuild().getContext();
	}
	else if (pDock == &getDockFind())
	{
		pLog = &getDockFind().getContext();
	}

	if (!pLog)
		return;

	rdoEditor::RDOEditorTabCtrl* pEditorTab = model->getTab();
	if (pEditorTab)
	{
		for (int i = 0; i < pEditorTab->count(); ++i)
		{
			pEditorTab->getItemEdit(i)->setLog(*pLog);
		}
	}
}

void RDOStudioMainFrame::onModelToolBarOrientationChanged(Qt::Orientation orientation)
{
	m_pModelSpeedSlider->setOrientation(orientation);
}
