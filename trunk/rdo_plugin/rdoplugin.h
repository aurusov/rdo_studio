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
typedef double (*PFunGetShowRate)();
typedef void (*PFunSetShowRate)( double value );
typedef void (*PFunShowNextFrame)();
typedef void (*PFunShowPrevFrame)();
typedef bool (*PFunCanShowNextFrame)();
typedef bool (*PFunCanShowPrevFrame)();
typedef int (*PFunGetFrameCount)();
typedef const char* (*PFunGetFrameName)( int index );
typedef void (*PFunShowFrame)( int index );
typedef void (*PFunCloseAllFrame)();

class Model {
public:
	Model(): hasModel( NULL ), build( NULL ), run( NULL ), stop( NULL ), isRunning( NULL ), getShowMode( NULL ), setShowMode( NULL ), isFrmDescribed( NULL ), getShowRate( NULL ), setShowRate( NULL ), showNextFrame( NULL ), showPrevFrame( NULL ), canShowNextFrame( NULL ), canShowPrevFrame( NULL ), getFrameCount( NULL ), getFrameName( NULL ), showFrame( NULL ), closeAllFrame( NULL ) {};
	virtual ~Model() {};

	PFunHasModel         hasModel;
	PFunBuild            build;
	PFunRun              run;
	PFunStop             stop;
	PFunIsRunning        isRunning;
	PFunGetShowMode      getShowMode;
	PFunSetShowMode      setShowMode;
	PFunIsFrmDescribed   isFrmDescribed;
	PFunGetShowRate      getShowRate;
	PFunSetShowRate      setShowRate;
	PFunShowNextFrame    showNextFrame;
	PFunShowPrevFrame    showPrevFrame;
	PFunCanShowNextFrame canShowNextFrame;
	PFunCanShowPrevFrame canShowPrevFrame;
	PFunGetFrameCount    getFrameCount;
	PFunGetFrameName     getFrameName;
	PFunShowFrame        showFrame;
	PFunCloseAllFrame    closeAllFrame;
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
