#include <windows.h>

#define RDOPLUGIN_EXPORTS
#include "rdoplugin.h"

int WINAPI DllMain( HINSTANCE hinst, DWORD fdwReason, PVOID pvReserved )
{
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: break;
		case DLL_PROCESS_DETACH: break;
		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH : break;
	}
	return TRUE;
}

void getPluginInfo( rdoPlugin::PluginInfo* info )
{
	static char* name         = "Test";
	static char* version_info = "";
	static char* description  = "some info about plugin";
	info->name = name;
	info->version_major  = 1;
	info->version_minor  = 0;
	info->version_build  = 1;
	info->version_info   = version_info;
	info->description    = description;
	info->defaultRunMode = rdoPlugin::prmNoAuto;
}

bool startPlugin( const rdoPlugin::Studio* studio )
{
	return true;
}

void stopPlugin()
{
}

const int enumMessages()
{
	return 0;
}

void pluginProc( const int message, const int param1 )
{
}

void trace( const char* line )
{
}

void results( const char* lines )
{
}
