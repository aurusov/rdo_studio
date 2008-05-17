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

void __stdcall getPluginInfo( rdoPlugin::PluginInfo* info )
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

bool __stdcall startPlugin( const rdoPlugin::Studio* studio )
{
	return true;
}

void __stdcall stopPlugin()
{
}

const int __stdcall enumMessages()
{
	return 0;
}

void __stdcall pluginProc( const int message, const int param1 )
{
}

void __stdcall trace( const char* line )
{
}

void __stdcall results( const char* lines )
{
}
