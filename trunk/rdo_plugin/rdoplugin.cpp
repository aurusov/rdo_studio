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
	static char* name         = "Test2";
	static char* version_info = "";
	static char* description  = "some info about plugin";
	info->name = name;
	info->version_major = 2;
	info->version_minor = 0;
	info->version_build = 1;
	info->version_info  = version_info;
	info->description   = description;
}

rdoPlugin::PluginRunMode getPluginRunMode()
{
	return rdoPlugin::prmStudioStartUp;
}
