#include "stdafx.h"
#include "rdopluginmfc.h"
#include "rdopluginmfcmainframe.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFC
// ----------------------------------------------------------------------------
RDOPluginMFC pluginMFCApp;

BEGIN_MESSAGE_MAP(RDOPluginMFC, CWinApp)
	//{{AFX_MSG_MAP(RDOPluginMFC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPluginMFC::RDOPluginMFC():
	CWinApp()
{
}

void getPluginInfo( rdoPlugin::PluginInfo* info )
{
	static char* name         = "Dialog";
	static char* version_info = "";
	static char* description  = "MFC Plugin";
	info->name = name;
	info->version_major  = 1;
	info->version_minor  = 0;
	info->version_build  = 1;
	info->version_info   = version_info;
	info->description    = description;
	info->defaultRunMode = rdoPlugin::prmNoAuto;
}

bool startPlugin( const rdoPlugin::Studio* _studio )
{
//	TRACE( "plugin handle = %d\n", AfxGetInstanceHandle() );
//	TRACE( "studio        = %d\n", &pluginMFCApp.studio );
//	TRACE( "studio.run    = %d\n", pluginMFCApp.studio.model.run );
	pluginMFCApp.studio = *_studio;
//	TRACE( "studio.run    = %d\n", pluginMFCApp.studio.model.run );
	if ( !pluginMFCApp.m_pMainWnd ) {
		RDOPluginMFCMainFrame* frame = new RDOPluginMFCMainFrame;
		pluginMFCApp.m_pMainWnd = frame;
		if ( !frame->LoadFrame( IDR_MAINFRAME ) ) {
			delete frame;
			return false;
		}
		frame->ShowWindow( SW_SHOW );
		frame->UpdateWindow();
	}
	return true;
}

void stopPlugin()
{
	if ( pluginMFCApp.m_pMainWnd && pluginMFCApp.m_pMainWnd->GetSafeHwnd() ) {
		pluginMFCApp.m_pMainWnd->DestroyWindow();
		delete pluginMFCApp.m_pMainWnd;
		pluginMFCApp.m_pMainWnd = NULL;
	}
}
