#ifndef RDOPLUGIN_H
#define RDOPLUGIN_H

#include <string>

#ifdef RDOPLUGIN_EXPORTS
#define RDOPLUGIN_DLL __declspec(dllexport)
#else
#define RDOPLUGIN_DLL __declspec(dllimport)
#endif

namespace rdoPlugin {

struct PluginInfo {
	int structSize;
	const char* name;
	int version_major;
	int version_minor;
	int version_build;
	const char* version_info;
	const char* description;
};

enum PluginState   { psStop, psRun };
enum PluginRunMode { prmNoAuto, prmStudioStartUp, prmModelStartUp };

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef PluginRunMode (*PFunGetPluginRunMode)();

};

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL rdoPlugin::PluginRunMode getPluginRunMode();
}

#endif // RDOPLUGIN_H
