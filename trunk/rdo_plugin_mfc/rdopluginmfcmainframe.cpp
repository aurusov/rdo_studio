#include "stdafx.h"
#include "rdopluginmfcmainframe.h"
#include "rdopluginmfc.h"
#include "resource.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOPluginMFCMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(RDOPluginMFCMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(RDOPluginMFCMainFrame)
	ON_COMMAND(ID_PLUGIN_CLOSE, OnPluginClose)
	ON_COMMAND(ID_MODEL_BUILD, OnModelBuild)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_COMMAND(ID_MODEL_STOP, OnModelStop)
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD, OnUpdateModelBuild)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP, OnUpdateModelStop)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODEL_RUN_NOSHOW, OnModelRunNoshow)
	ON_COMMAND(ID_MODEL_RUN_ANIMATION, OnModelRunAnimation)
	ON_COMMAND(ID_MODEL_RUN_MONITOR, OnModelRunMonitor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_NOSHOW, OnUpdateModelRunNoshow)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_ANIMATION, OnUpdateModelRunAnimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_MONITOR, OnUpdateModelRunMonitor)
	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowrateInc)
	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowrateIncFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowrateDecFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowrateDec)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowrateInc)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowrateIncFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowrateDecFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowrateDec)
	ON_COMMAND(ID_MODEL_FRAME_NEXT, OnModelFrameNext)
	ON_COMMAND(ID_MODEL_FRAME_PREV, OnModelFramePrev)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_NEXT, OnUpdateModelFrameNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_PREV, OnUpdateModelFramePrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPluginMFCMainFrame::RDOPluginMFCMainFrame()
{
}

RDOPluginMFCMainFrame::~RDOPluginMFCMainFrame()
{
}

BOOL RDOPluginMFCMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle |= WS_EX_TOPMOST;
	return TRUE;
}

int RDOPluginMFCMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetWindowPos( &CWnd::wndTopMost, 0, 0, 300, 200, SWP_NOMOVE );

	return 0;
}

BOOL RDOPluginMFCMainFrame::DestroyWindow()
{
	pluginMFCApp.PostThreadMessage( rdoPlugin::PLUGIN_MUSTEXIT_MESSAGE, reinterpret_cast<WPARAM>(AfxGetInstanceHandle()), 0 );
	return CFrameWnd::DestroyWindow();
}

void RDOPluginMFCMainFrame::OnPluginClose()
{
	OnClose();
}

void RDOPluginMFCMainFrame::OnModelBuild()
{
	pluginMFCApp.studio.model.build();
}

void RDOPluginMFCMainFrame::OnModelRun()
{
	pluginMFCApp.studio.model.run();
}

void RDOPluginMFCMainFrame::OnModelStop()
{
	pluginMFCApp.studio.model.stop();
}

void RDOPluginMFCMainFrame::OnUpdateModelBuild( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelRun( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelStop( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnModelRunNoshow() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::NoShow );
}

void RDOPluginMFCMainFrame::OnModelRunAnimation() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::Animation );
}

void RDOPluginMFCMainFrame::OnModelRunMonitor() 
{
	pluginMFCApp.studio.model.setShowMode( rdoPlugin::Monitor );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunNoshow(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::NoShow : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunAnimation(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::Animation : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunMonitor(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getShowMode() == rdoPlugin::Monitor : 0 );
}

void RDOPluginMFCMainFrame::OnModelShowrateInc() 
{
	double showRate = pluginMFCApp.studio.model.getShowRate();
	showRate *= 1.5;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.model.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowrateIncFour() 
{
	double showRate = pluginMFCApp.studio.model.getShowRate();
	showRate *= 4;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.model.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowrateDecFour() 
{
	double showRate = pluginMFCApp.studio.model.getShowRate();
	showRate /= 4;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.model.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowrateDec() 
{
	double showRate = pluginMFCApp.studio.model.getShowRate();
	showRate /= 1.5;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.model.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnUpdateModelShowrateInc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.model.getShowRate() * 1.5 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowrateIncFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.model.getShowRate() * 4 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowrateDecFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.model.getShowRate() / 4 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowrateDec(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getShowMode() != rdoPlugin::NoShow && pluginMFCApp.studio.model.getShowRate() / 1.5 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnModelFrameNext() 
{
	pluginMFCApp.studio.model.showNextFrame();
}

void RDOPluginMFCMainFrame::OnModelFramePrev() 
{
	pluginMFCApp.studio.model.showPrevFrame();
}

void RDOPluginMFCMainFrame::OnUpdateModelFrameNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.canShowNextFrame() );
}

void RDOPluginMFCMainFrame::OnUpdateModelFramePrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.canShowPrevFrame() );
}
