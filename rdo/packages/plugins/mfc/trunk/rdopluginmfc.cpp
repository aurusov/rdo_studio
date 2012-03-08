#include "app/rdo_studio_mfc/plugins/mfc/stdafx.h"
#include "app/rdo_studio_mfc/plugins/mfc/rdopluginmfc.h"
#include "app/rdo_studio_mfc/plugins/mfc/rdopluginmfcmainframe.h"
#include "app/rdo_studio_mfc/plugins/mfc/resource.h"

#include <string>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOPluginMFC
// --------------------------------------------------------------------------------
RDOPluginMFC pluginMFCApp;

BEGIN_MESSAGE_MAP(RDOPluginMFC, CWinApp)
END_MESSAGE_MAP()

RDOPluginMFC::RDOPluginMFC():
	CWinApp(),
	frame( NULL )
{
}

void __stdcall getPluginInfo( rdoPlugin::PluginInfo* info )
{
	TRACE( "1. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	static char* name         = "MFC Plugin";
	static char* version_info = "";
	static char* description  = "MFC-Based Plugin";
	info->name = name;
	info->version_major  = 1;
	info->version_minor  = 0;
	info->version_build  = 1;
	info->version_info   = version_info;
	info->description    = description;
	info->defaultRunMode = rdoPlugin::prmNoAuto;
}

bool __stdcall startPlugin( const rdoPlugin::Studio* _studio )
{
	TRACE( "2. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	pluginMFCApp.studio = *_studio;
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

void __stdcall stopPlugin()
{
	TRACE( "3. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	if ( pluginMFCApp.m_pMainWnd && pluginMFCApp.m_pMainWnd->GetSafeHwnd() ) {
		pluginMFCApp.m_pMainWnd->DestroyWindow();
		delete pluginMFCApp.m_pMainWnd;
		pluginMFCApp.m_pMainWnd = NULL;
		pluginMFCApp.frame = NULL;
	}
}

const int __stdcall enumMessages()
{
	TRACE( "4. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	static int i = 0;
	int messages[] = { rdoPlugin::PM_MODEL_NEW, rdoPlugin::PM_MODEL_OPEN, rdoPlugin::PM_MODEL_SAVE, rdoPlugin::PM_MODEL_CLOSE, rdoPlugin::PM_MODEL_NAME_CHANGED, rdoPlugin::PM_MODEL_MODIFY, rdoPlugin::PM_MODEL_BUILD_OK, rdoPlugin::PM_MODEL_BUILD_FAILD, rdoPlugin::PM_MODEL_BEFORE_START, rdoPlugin::PM_MODEL_AFTER_START, rdoPlugin::PM_MODEL_FINISHED, rdoPlugin::PM_MODEL_STOP_CANCEL, rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR, rdoPlugin::PM_MODEL_RUNTIMEMODE };
	static const int cnt = sizeof( messages )/sizeof( int );
	if ( i < cnt ) {
		return messages[i++];
	}
	return 0;
}

void __stdcall pluginProc( const int message, void* param1 )
{
	UNUSED(param1);

	TRACE( "5. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	switch ( message ) {
		case rdoPlugin::PM_MODEL_NEW: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_NEW" );
			break;
		}
		case rdoPlugin::PM_MODEL_OPEN: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_OPEN" );
			break;
		}
		case rdoPlugin::PM_MODEL_SAVE: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_SAVE" );
			break;
		}
		case rdoPlugin::PM_MODEL_CLOSE: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_CLOSE" );
			break;
		}
		case rdoPlugin::PM_MODEL_NAME_CHANGED: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_NAME_CHANGED" );
			break;
		}
		case rdoPlugin::PM_MODEL_MODIFY: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_MODIFY" );
			break;
		}
		case rdoPlugin::PM_MODEL_BUILD_OK: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_BUILD_OK" );
			break;
		}
		case rdoPlugin::PM_MODEL_BUILD_FAILD: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_BUILD_FAILD" );
			break;
		}
		case rdoPlugin::PM_MODEL_BEFORE_START: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_BEFORE_START" );
			std::string str = pluginMFCApp.studio.model.getStructure();
			std::string::size_type pos = str.find( '\n' );
			if ( pos != std::string::npos ) {
				while ( pos != std::string::npos ) {
					std::string str2( &str[0], 0, pos );
					pluginMFCApp.frame->insertLine( str2.c_str() );
					str.erase( 0, pos + 1 );
					pos = str.find( '\n' );
				}
				std::string str2( &str[0], 0, pos );
				if ( !str2.empty() ) {
					pluginMFCApp.frame->insertLine( str2.c_str() );
				}
			} else {
				pluginMFCApp.frame->insertLine( str.c_str() );
			}
			break;
		}
		case rdoPlugin::PM_MODEL_AFTER_START: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_AFTER_START" );
			break;
		}
		case rdoPlugin::PM_MODEL_FINISHED: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_FINISHED" );
			break;
		}
		case rdoPlugin::PM_MODEL_STOP_CANCEL: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_STOP_CANCEL" );
			break;
		}
		case rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_STOP_RUNTIME_ERROR" );
			break;
		}
		case rdoPlugin::PM_MODEL_RUNTIMEMODE: {
			pluginMFCApp.frame->insertLine( "PM_MODEL_RUNTIMEMODE" );
			break;
		}
	}
}

void __stdcall trace( const char* line )
{
	TRACE( "6. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	pluginMFCApp.frame->insertLine( line );
}

void __stdcall results( const char* lines )
{
	TRACE( "7. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	std::string str = lines;
	std::string::size_type pos = str.find( '\n' );
	if ( pos != std::string::npos ) {
		while ( pos != std::string::npos ) {
			std::string str2( &str[0], 0, pos );
			pluginMFCApp.frame->insertLine( str2.c_str() );
			str.erase( 0, pos + 1 );
			pos = str.find( '\n' );
		}
		std::string str2( &str[0], 0, pos );
		if ( !str2.empty() ) {
			pluginMFCApp.frame->insertLine( str2.c_str() );
		}
	} else {
		pluginMFCApp.frame->insertLine( str.c_str() );
	}
}
