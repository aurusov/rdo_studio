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

const int enumMessages()
{
	static int i = 0;
	static const int cnt = 9;
	int messages[ cnt ] = { rdoPlugin::PM_MODEL_NEW, rdoPlugin::PM_MODEL_OPEN, rdoPlugin::PM_MODEL_SAVE, rdoPlugin::PM_MODEL_CLOSE, rdoPlugin::PM_MODEL_MODIFY, rdoPlugin::PM_MODEL_BUILD, rdoPlugin::PM_MODEL_START, rdoPlugin::PM_MODEL_STOP, rdoPlugin::PM_MODEL_SHOWMODE };
	if ( i < cnt ) {
		return messages[i++];
	}
	return 0;
}

void pluginProc( const int message )
{
	if ( message == rdoPlugin::PM_MODEL_NEW ) {
		TRACE( "PM_MODEL_NEW\n" );
	} else if ( message == rdoPlugin::PM_MODEL_OPEN ) {
		TRACE( "PM_MODEL_OPEN\n" );
	} else if ( message == rdoPlugin::PM_MODEL_SAVE ) {
		TRACE( "PM_MODEL_SAVE\n" );
	} else if ( message == rdoPlugin::PM_MODEL_CLOSE ) {
		TRACE( "PM_MODEL_CLOSE\n" );
	} else if ( message == rdoPlugin::PM_MODEL_MODIFY ) {
		TRACE( "PM_MODEL_MODIFY\n" );
	} else if ( message == rdoPlugin::PM_MODEL_BUILD ) {
		TRACE( "PM_MODEL_BUILD\n" );
	} else if ( message == rdoPlugin::PM_MODEL_START ) {
		TRACE( "PM_MODEL_START\n" );
	} else if ( message == rdoPlugin::PM_MODEL_STOP ) {
		TRACE( "PM_MODEL_STOP\n" );
	} else if ( message == rdoPlugin::PM_MODEL_SHOWMODE ) {
		TRACE( "PM_MODEL_SHOWMODE\n" );
	}
}
