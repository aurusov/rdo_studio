#ifndef RDOPLUGIN_H
#define RDOPLUGIN_H

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

enum PluginState   { psStoped, psActive };
enum PluginRunMode { prmNoAuto, prmStudioStartUp, prmModelStartUp };

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef PluginRunMode (*PFunGetPluginRunMode)();
typedef bool (*PFunStartPlugin)();
typedef void (*PFunStopPlugin)();

};

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL rdoPlugin::PluginRunMode getPluginRunMode();
	RDOPLUGIN_DLL bool startPlugin();
	RDOPLUGIN_DLL void stopPlugin();
}

#endif // RDOPLUGIN_H
