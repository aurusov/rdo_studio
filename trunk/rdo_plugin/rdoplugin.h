#ifndef RDOPLUGIN_H
#define RDOPLUGIN_H

#include <string>

#ifdef RDOPLUGIN_EXPORTS
#define RDOPLUGIN_DLL __declspec(dllexport)
#else
#define RDOPLUGIN_DLL __declspec(dllimport)
#endif

struct PluginInfo {
	int structSize;
	const char* name;
	int version_major;
	int version_minor;
	int version_build;
	const char* version_info;
	const char* description;
};

typedef void (*PFunGetPluginInfo)( PluginInfo* );

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( PluginInfo* info );
}

#endif // RDOPLUGIN_H
