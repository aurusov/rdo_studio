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
#include <boost/bind.hpp>
#include <QtCore/qprocess.h>
#include <QtGui/qmdisubwindow.h>
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
RDOStudioMainFrame::RDOStudioMainFrame()
	: m_updateTimerID(0)
{
	setupUi(this);
	mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);

	createStatusBar();
	createToolBar  ();

	connect(menuFileReopen, SIGNAL(triggered(QAction*)), this, SLOT(onMenuFileReopen(QAction*)));
	connect(actFileExit,    SIGNAL(triggered(bool)),     this, SLOT(close()));

	connect(actViewSettings, SIGNAL(triggered(bool)), this, SLOT(onViewOptions()));
	connect(actHelpContext,  SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));
	connect(actHelpAbout,    SIGNAL(triggered(bool)), this, SLOT(onHelpAbout  ()));

	connect(toolBarModel, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onToolBarModelOrientationChanged(Qt::Orientation)));

	Scintilla_LinkLexers();

	loadMenuFileReopen  ();
	updateMenuFileReopen();
}

//! todo не вызывается диструктор
RDOStudioMainFrame::~RDOStudioMainFrame()
{}

void RDOStudioMainFrame::createStatusBar()
{
	m_pStatusBar = rdo::Factory<StatusBar>::create(this);
}

void RDOStudioMainFrame::createToolBar()
{
	m_pModelSpeedSlider = new QSlider(Qt::Horizontal, this);
	m_pModelSpeedSlider->setRange(0, 100);
	m_pModelSpeedSlider->setValue(100);
	m_pModelSpeedSlider->setMaximumSize(100, 100);

	toolBarModel->insertWidget(actModelFrameNext, m_pModelSpeedSlider);
	toolBarModel->insertSeparator(actModelFrameNext);
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
	pMenuToolbarView->addAction(toolBarFile->toggleViewAction());
	pMenuToolbarView->addAction(toolBarEdit->toggleViewAction());
	pMenuToolbarView->addAction(toolBarZoom->toggleViewAction());
	pMenuToolbarView->addAction(toolBarModel->toggleViewAction());

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
	if (model && !model->closeModel())
	{
		event->ignore();
	}

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

CREF(LPStatusBar) RDOStudioMainFrame::statusBar() const
{
	return m_pStatusBar;
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

void RDOStudioMainFrame::onToolBarModelOrientationChanged(Qt::Orientation orientation)
{
	m_pModelSpeedSlider->setOrientation(orientation);
}

void RDOStudioMainFrame::onMenuFileReopen(QAction* pAction)
{
	tstring fileName = pAction->text().toStdString();
	tstring::size_type pos = fileName.find(' ');
	if (pos == tstring::npos)
		return;

	fileName = fileName.substr(pos + 1);
	if (!model->openModel(fileName) && model->isPrevModelClosed())
	{
		ReopenList::iterator it = std::find(m_reopenList.begin(), m_reopenList.end(), pAction->text().toStdString());
		if (it != m_reopenList.end())
		{
			m_reopenList.erase(it);
		}
		updateMenuFileReopen();
	}
}

void RDOStudioMainFrame::insertMenuFileReopenItem(CREF(tstring) item)
{
	if (!item.empty())
	{
		STL_FOR_ALL(m_reopenList, it)
		{
			if (*it == item)
			{
				m_reopenList.erase(it);
				break;
			}
		}

		m_reopenList.insert(m_reopenList.begin(), item);

		while (m_reopenList.size() > 10)
		{
			ReopenList::iterator it = m_reopenList.end();
			--it;
			m_reopenList.erase(it);
		}

		updateMenuFileReopen();
	}
}

void RDOStudioMainFrame::updateMenuFileReopen()
{
	menuFileReopen->clear();

	for (ReopenList::size_type reopenIndex = 0; reopenIndex < m_reopenList.size(); ++reopenIndex)
	{
		if (reopenIndex == 4)
		{
			menuFileReopen->addSeparator();
		}
		menuFileReopen->addAction(rdo::format("%d. %s", reopenIndex+1, m_reopenList[reopenIndex].c_str()).c_str());
	}

	menuFileReopen->setEnabled(!menuFileReopen->isEmpty());

	saveMenuFileReopen();
}

void RDOStudioMainFrame::loadMenuFileReopen()
{
	m_reopenList.clear();
	for (ruint i = 0; i < 10; i++)
	{
		tstring sec;
		if (i+1 < 10)
		{
			sec = rdo::format(_T("0%d"), i+1);
		}
		else
		{
			sec = rdo::format(_T("%d"), i+1);
		}
		TRY
		{
			tstring s = AfxGetApp()->GetProfileString(_T("reopen"), sec.c_str(), _T(""));
			if (!s.empty())
			{
				m_reopenList.insert(m_reopenList.end(), s);
			}
		}
		CATCH(CException, e)
		{}
		END_CATCH
	}
}

void RDOStudioMainFrame::saveMenuFileReopen() const
{
	for (ReopenList::size_type i = 0; i < 10; i++)
	{
		tstring sec;
		if (i+1 < 10)
		{
			sec = rdo::format(_T("0%d"), i+1);
		}
		else
		{
			sec = rdo::format(_T("%d"), i+1);
		}
		tstring s;
		if (i < m_reopenList.size())
		{
			s = m_reopenList[i];
		}
		else
		{
			s = _T("");
		}
		TRY
		{
			AfxGetApp()->WriteProfileString(_T("reopen"), sec.c_str(), s.c_str());
		}
		CATCH(CException, e)
		{}
		END_CATCH
	}
}
