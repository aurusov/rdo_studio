#include "stdafx.h"
#include "rdopluginmfc.h"
#include "rdopluginmfcdialog.h"

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

static RDOPluginMFCDialog* dlg = NULL;

bool startPlugin( const rdoPlugin::Studio* _studio )
{
	TRACE( "plugin handle    = %d\n", AfxGetInstanceHandle() );
	TRACE( "studio           = %d\n", &pluginMFCApp.studio );
	TRACE( "studio.runModel1 = %d\n", pluginMFCApp.studio.runModel );
	pluginMFCApp.studio = *_studio;
	TRACE( "studio.runModel2 = %d\n", pluginMFCApp.studio.runModel );
	if ( !dlg ) {
		dlg = new RDOPluginMFCDialog;
		dlg->Create( IDD_PLUGIN_DIALOG, CWnd::FromHandle( ::GetDesktopWindow() ) );
//		dlg->SetIcon( pluginMFCApp.LoadIcon( MAKEINTRESOURCE(IDD_PLUGIN_DIALOG) ), TRUE );
		dlg->SetIcon( (HICON)::LoadImage( pluginMFCApp.m_hInstance, MAKEINTRESOURCE(IDD_PLUGIN_DIALOG), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR ), TRUE );
	}
	return true;
}

void stopPlugin()
{
	if ( dlg ) {
		dlg->DestroyWindow();
		delete dlg;
		dlg = NULL;
	}
}
