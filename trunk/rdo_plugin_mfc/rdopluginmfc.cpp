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
	CWinApp(),
	frame( NULL )
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
		pluginMFCApp.frame = new RDOPluginMFCMainFrame;
		pluginMFCApp.m_pMainWnd = pluginMFCApp.frame;
		if ( !pluginMFCApp.frame->LoadFrame( IDR_MAINFRAME ) ) {
			delete pluginMFCApp.frame;
			pluginMFCApp.frame = NULL;
			return false;
		}
		pluginMFCApp.frame->ShowWindow( SW_SHOW );
		pluginMFCApp.frame->UpdateWindow();
	}
	return true;
}

void stopPlugin()
{
	if ( pluginMFCApp.m_pMainWnd && pluginMFCApp.m_pMainWnd->GetSafeHwnd() ) {
		pluginMFCApp.m_pMainWnd->DestroyWindow();
		delete pluginMFCApp.m_pMainWnd;
		pluginMFCApp.m_pMainWnd = NULL;
		pluginMFCApp.frame = NULL;
	}
}

const int enumMessages()
{
	static int i = 0;
	static const int cnt = 14;
	int messages[ cnt ] = { rdoPlugin::PM_MODEL_NEW, rdoPlugin::PM_MODEL_OPEN, rdoPlugin::PM_MODEL_SAVE, rdoPlugin::PM_MODEL_CLOSE, rdoPlugin::PM_MODEL_NAME_CHANGED, rdoPlugin::PM_MODEL_MODIFY, rdoPlugin::PM_MODEL_BUILD_OK, rdoPlugin::PM_MODEL_BUILD_FAILD, rdoPlugin::PM_MODEL_BEFORE_START, rdoPlugin::PM_MODEL_AFTER_START, rdoPlugin::PM_MODEL_FINISHED, rdoPlugin::PM_MODEL_STOP_CANCEL, rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR, rdoPlugin::PM_MODEL_SHOWMODE };
	if ( i < cnt ) {
		return messages[i++];
	}
	return 0;
}

void pluginProc( const int message )
{
	if ( message == rdoPlugin::PM_MODEL_NEW ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_NEW" );
	} else if ( message == rdoPlugin::PM_MODEL_OPEN ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_OPEN" );
	} else if ( message == rdoPlugin::PM_MODEL_SAVE ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_SAVE" );
	} else if ( message == rdoPlugin::PM_MODEL_CLOSE ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_CLOSE" );
	} else if ( message == rdoPlugin::PM_MODEL_NAME_CHANGED ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_NAME_CHANGED" );
	} else if ( message == rdoPlugin::PM_MODEL_MODIFY ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_MODIFY" );
	} else if ( message == rdoPlugin::PM_MODEL_BUILD_OK ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_BUILD_OK" );
	} else if ( message == rdoPlugin::PM_MODEL_BUILD_FAILD ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_BUILD_FAILD" );
	} else if ( message == rdoPlugin::PM_MODEL_BEFORE_START ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_BEFORE_START" );
	} else if ( message == rdoPlugin::PM_MODEL_AFTER_START ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_AFTER_START" );
	} else if ( message == rdoPlugin::PM_MODEL_FINISHED ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_FINISHED" );
	} else if ( message == rdoPlugin::PM_MODEL_STOP_CANCEL ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_STOP_CANCEL" );
	} else if ( message == rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_STOP_RUNTIME_ERROR" );
	} else if ( message == rdoPlugin::PM_MODEL_SHOWMODE ) {
		pluginMFCApp.frame->insertLine( "PM_MODEL_SHOWMODE" );
	}
}
