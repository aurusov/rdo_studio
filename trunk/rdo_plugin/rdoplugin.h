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
enum ModelShowMode { NoShow, Animation, Monitor };

struct PluginInfo {
	const char* name;
	int version_major;
	int version_minor;
	int version_build;
	const char* version_info;
	const char* description;
	PluginRunMode defaultRunMode;
};

typedef bool (*PFunHasModel)();
typedef void (*PFunBuild)();
typedef void (*PFunRun)();
typedef void (*PFunStop)();
typedef bool (*PFunIsRunning)();
typedef ModelShowMode (*PFunGetShowMode)();
typedef void (*PFunSetShowMode)( ModelShowMode showMode );
typedef bool (*PFunIsFrmDescribed)();

class Model {
public:
	Model(): hasModel( NULL ), build( NULL ), run( NULL ), stop( NULL ), isRunning( NULL ), getShowMode( NULL ), setShowMode( NULL ), isFrmDescribed( NULL ) {};
	virtual ~Model() {};

	PFunHasModel       hasModel;
	PFunBuild          build;
	PFunRun            run;
	PFunStop           stop;
	PFunIsRunning      isRunning;
	PFunGetShowMode    getShowMode;
	PFunSetShowMode    setShowMode;
	PFunIsFrmDescribed isFrmDescribed;
};

class Studio {
public:
	Studio() {};
	virtual ~Studio() {};

	Model model;
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
