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

typedef void (*PFunNewModel)();
typedef bool (*PFunOpenModel)( const char* modelName );
typedef void (*PFunSaveModel)();
typedef void (*PFunCloseModel)();
typedef bool (*PFunHasModel)();
typedef bool (*PFunIsModelModify)();
typedef void (*PFunBuild)();
typedef void (*PFunRun)();
typedef void (*PFunStop)();
typedef bool (*PFunIsRunning)();
typedef ModelShowMode (*PFunGetShowMode)();
typedef void (*PFunSetShowMode)( ModelShowMode showMode );

class Model {
public:
	Model(): newModel( NULL ), openModel( NULL ), saveModel( NULL ), closeModel( NULL ), hasModel( NULL ), isModify( NULL ), build( NULL ), run( NULL ), stop( NULL ), isRunning( NULL ), getShowMode( NULL ), setShowMode( NULL ) {};
	virtual ~Model() {};

	PFunNewModel         newModel;
	PFunOpenModel        openModel;
	PFunSaveModel        saveModel;
	PFunCloseModel       closeModel;
	PFunHasModel         hasModel;
	PFunIsModelModify    isModify;
	PFunBuild            build;
	PFunRun              run;
	PFunStop             stop;
	PFunIsRunning        isRunning;
	PFunGetShowMode      getShowMode;
	PFunSetShowMode      setShowMode;
};

typedef bool (*PFunIsFrameDescribed)();
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

class Frame {
public:
	Frame(): isDescribed( NULL ), getShowRate( NULL ), setShowRate( NULL ), showNext( NULL ), showPrev( NULL ), canShowNext( NULL ), canShowPrev( NULL ), getCount( NULL ), getName( NULL ), show( NULL ), closeAll( NULL ) {};
	virtual ~Frame() {};

	PFunIsFrameDescribed isDescribed;
	PFunGetShowRate      getShowRate;
	PFunSetShowRate      setShowRate;
	PFunShowNextFrame    showNext;
	PFunShowPrevFrame    showPrev;
	PFunCanShowNextFrame canShowNext;
	PFunCanShowPrevFrame canShowPrev;
	PFunGetFrameCount    getCount;
	PFunGetFrameName     getName;
	PFunShowFrame        show;
	PFunCloseAllFrame    closeAll;
};

class Studio {
public:
	Studio() {};
	virtual ~Studio() {};

	Model model;
	Frame frame;
};

static const int PLUGIN_MUSTEXIT_MESSAGE = ::RegisterWindowMessage( "PLUGIN_MUSTEXIT_MESSAGE" );
static const int PM_MODEL_NEW      = ::RegisterWindowMessage( "PM_MODEL_NEW_MESSAGE" );
static const int PM_MODEL_OPEN     = ::RegisterWindowMessage( "PM_MODEL_OPEN_MESSAGE" );
static const int PM_MODEL_SAVE     = ::RegisterWindowMessage( "PM_MODEL_SAVE_MESSAGE" );
static const int PM_MODEL_CLOSE    = ::RegisterWindowMessage( "PM_MODEL_CLOSE_MESSAGE" );
static const int PM_MODEL_MODIFY   = ::RegisterWindowMessage( "PM_MODEL_MODIFY_MESSAGE" );
static const int PM_MODEL_BUILD    = ::RegisterWindowMessage( "PM_MODEL_BUILD_MESSAGE" );
static const int PM_MODEL_START    = ::RegisterWindowMessage( "PM_MODEL_START_MESSAGE" );
static const int PM_MODEL_STOP     = ::RegisterWindowMessage( "PM_MODEL_STOP_MESSAGE" );
static const int PM_MODEL_SHOWMODE = ::RegisterWindowMessage( "PM_MODEL_SHOWMODE_MESSAGE" );

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef bool (*PFunStartPlugin)( const Studio* studio );
typedef void (*PFunStopPlugin)();
typedef const int (*PFunEnumMessages)();
typedef void (*PFunPluginProc)( const int );

};

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL bool startPlugin( const rdoPlugin::Studio* studio );
	RDOPLUGIN_DLL void stopPlugin();
	RDOPLUGIN_DLL const int enumMessages();
	RDOPLUGIN_DLL void pluginProc( const int message );
}

#endif // RDOPLUGIN_H
