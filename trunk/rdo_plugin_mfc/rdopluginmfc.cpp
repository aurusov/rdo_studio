#include "stdafx.h"
#include "rdopluginmfc.h"

#define RDOPLUGIN_EXPORTS
#include <rdoplugin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFC
// ----------------------------------------------------------------------------
RDOPluginMFC theApp;

BEGIN_MESSAGE_MAP(RDOPluginMFC, CWinApp)
	//{{AFX_MSG_MAP(RDOPluginMFC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPluginMFC::RDOPluginMFC()
{
}

void getPluginInfo( rdoPlugin::PluginInfo* info )
{
	static char* name         = "Dialog";
	static char* version_info = "";
	static char* description  = "MFC Plugin";
	info->name = name;
	info->version_major = 1;
	info->version_minor = 0;
	info->version_build = 1;
	info->version_info  = version_info;
	info->description   = description;
}

rdoPlugin::PluginRunMode getPluginRunMode()
{
	return rdoPlugin::prmNoAuto;
}

static CDialog* dlg = NULL;

bool startPlugin()
{
	if ( !dlg ) {
		dlg = new CDialog;
		dlg->Create( IDD_DIALOG, CWnd::FromHandle( ::GetDesktopWindow() ) );
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
