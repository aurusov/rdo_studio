#ifndef RDOPLUGIN_H
#define RDOPLUGIN_H

#ifdef RDOPLUGIN_EXPORTS
#define RDOPLUGIN_DLL __declspec(dllexport)
#else
#define RDOPLUGIN_DLL __declspec(dllimport)
#endif

namespace rdoPlugin {

enum PluginState   { psStoped, psActive };
enum PluginRunMode { prmNoAuto, prmStudioStartUp, prmModelStartUp };

struct PluginInfo {
	const char* name;
	int version_major;
	int version_minor;
	int version_build;
	const char* version_info;
	const char* description;
	PluginRunMode defaultRunMode;
};

typedef void (*PFunRunModel)();
typedef bool (*PFunIsModelRunning)();

class Studio {
public:
	Studio(): runModel( NULL ) {};
	virtual ~Studio() {};

	PFunRunModel       runModel;
	PFunIsModelRunning isModelRunning;
};

static const int PLUGIN_MUSTEXIT_MESSAGE = ::RegisterWindowMessage( "PLUGIN_MUSTEXIT_MESSAGE" );

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef bool (*PFunStartPlugin)( const Studio* studio );
typedef void (*PFunStopPlugin)();

};

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL bool startPlugin( const rdoPlugin::Studio* studio );
	RDOPLUGIN_DLL void stopPlugin();
}

#endif // RDOPLUGIN_H
