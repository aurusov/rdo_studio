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
enum ModelRuntimeMode {
	MRTM_MaxSpeed,
	MRTM_Jump,
	MRTM_Sync,
	MRTM_Pause
};

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
typedef ModelRuntimeMode (*PFunGetRuntimeMode)();
typedef void (*PFunSetRuntimeMode)( ModelRuntimeMode runtimeMode );
typedef const char* (*PFunGetModelStructure)();

class Model {
public:
	Model(): newModel( NULL ), openModel( NULL ), saveModel( NULL ), closeModel( NULL ), hasModel( NULL ), isModify( NULL ), build( NULL ), run( NULL ), stop( NULL ), isRunning( NULL ), getRuntimeMode( NULL ), setRuntimeMode( NULL ), getStructure( NULL ) {};
	virtual ~Model() {};

	PFunNewModel          newModel;
	PFunOpenModel         openModel;
	PFunSaveModel         saveModel;
	PFunCloseModel        closeModel;
	PFunHasModel          hasModel;
	PFunIsModelModify     isModify;
	PFunBuild             build;
	PFunRun               run;
	PFunStop              stop;
	PFunIsRunning         isRunning;
	PFunGetRuntimeMode    getRuntimeMode;
	PFunSetRuntimeMode    setRuntimeMode;
	PFunGetModelStructure getStructure;
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

typedef void (*PFunPluginStop)( const HMODULE );
typedef bool (*PFunPluginIsStoped)( const HMODULE );

class Plugin {
public:
	Plugin(): stop( NULL ), isStoped( NULL ) {};
	virtual ~Plugin() {};

	PFunPluginStop     stop;
	PFunPluginIsStoped isStoped;
};

typedef void (*PFunShow)( int cmdShow );
typedef bool (*PFunIsShow)();
typedef HWND (*PFunGetMainFrame)();

class Studio {
public:
	Studio(): show( NULL ), isShow( NULL ), mainFrame( NULL ) {};
	virtual ~Studio() {};

	Model  model;
	Frame  frame;
	Plugin plugin;

	PFunShow             show;
	PFunIsShow           isShow;
	PFunGetMainFrame     mainFrame;
};

enum RDOPluginMessage {
	PM_MODEL_NEW = 1,
	PM_MODEL_OPEN,
	PM_MODEL_SAVE,
	PM_MODEL_CLOSE,
	PM_MODEL_NAME_CHANGED,
	PM_MODEL_MODIFY,
	PM_MODEL_BUILD_OK,
	PM_MODEL_BUILD_FAILD,
	PM_MODEL_BEFORE_START,
	PM_MODEL_AFTER_START,
	PM_MODEL_FINISHED,
	PM_MODEL_STOP_CANCEL,
	PM_MODEL_STOP_RUNTIME_ERROR,
	PM_MODEL_RUNTIMEMODE
};

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef bool (*PFunStartPlugin)( const Studio* studio );
typedef void (*PFunStopPlugin)();
typedef const int (*PFunEnumMessages)();
typedef void (*PFunPluginProc)( const int );
typedef void (*PFunTrace)( const char* );
typedef void (*PFunResults)( const char* );

};

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL bool startPlugin( const rdoPlugin::Studio* studio );
	RDOPLUGIN_DLL void stopPlugin();
	RDOPLUGIN_DLL const int enumMessages();
	RDOPLUGIN_DLL void pluginProc( const int message );
	RDOPLUGIN_DLL void trace( const char* line );
	RDOPLUGIN_DLL void results( const char* lines );
}

#endif // RDOPLUGIN_H
