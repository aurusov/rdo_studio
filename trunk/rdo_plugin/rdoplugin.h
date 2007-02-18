#ifndef RDOPLUGIN_H
#define RDOPLUGIN_H

#ifdef RDOPLUGIN_EXPORTS
#define RDOPLUGIN_DLL __declspec(dllexport)
#else
#define RDOPLUGIN_DLL __declspec(dllimport)
#endif

namespace rdoPlugin {

enum PluginState   {
	psStoped,
	psActive
};

enum PluginRunMode {
	prmNoAuto,
	prmStudioStartUp,
	prmModelStartUp
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

// ----------------------------------------------------------------------------
// ---------- ModelAction
// ----------------------------------------------------------------------------
// Класс для контроля действий над моделью: разрешает или запрещает дейсвтия.
//
enum ModelActionType {
	maCreate,
	maOpen,
	maSave,
	maClose,
	maBuild,
	maRun
};

typedef void (*PFunModelActionEnable)( ModelActionType action );
typedef void (*PFunModelActionDisable)( ModelActionType action );
typedef bool (*PFunModelActionState)( ModelActionType action );

class ModelAction {
public:
	ModelAction(): enable( NULL ), disable( NULL ), state( NULL ) {};
	virtual ~ModelAction() {};

	PFunModelActionEnable  enable;
	PFunModelActionDisable disable;
	PFunModelActionState   state;
};

// ----------------------------------------------------------------------------
// ---------- Model
// ----------------------------------------------------------------------------
// Класс модель: используется для создания, открытия, записи и запуска модели.
// Позволяет изменять содержимое отдельных файлов.
//
enum ModelRuntimeMode {
	MRTM_MaxSpeed,
	MRTM_Jump,
	MRTM_Sync,
	MRTM_Pause
};

enum ModelFileType { PAT = 0, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC };

typedef bool (*PFunCreateModel)( const char* modelName, const char* modelPath );
typedef bool (*PFunOpenModel)( const char* modelName );
typedef bool (*PFunSaveModel)();
typedef bool (*PFunCloseModel)();
typedef bool (*PFunHasModel)();
typedef bool (*PFunIsModelModify)();
typedef bool (*PFunBuildModel)();
typedef bool (*PFunRunModel)();
typedef bool (*PFunStopModel)();
typedef bool (*PFunIsRunning)();
typedef ModelRuntimeMode (*PFunGetRuntimeMode)();
typedef void (*PFunSetRuntimeMode)( ModelRuntimeMode runtimeMode );
typedef const char* (*PFunGetModelStructure)();
typedef bool (*PFunReadFile)( ModelFileType file_type, char** data );
typedef bool (*PFunWriteFile)( ModelFileType file_type, const char* data );

class Model {
public:
	Model(): create( NULL ), open( NULL ), save( NULL ), close( NULL ), hasModel( NULL ), isModify( NULL ), build( NULL ), run( NULL ), stop( NULL ), isRunning( NULL ), getRuntimeMode( NULL ), setRuntimeMode( NULL ), getStructure( NULL ), read( NULL ), write( NULL ) {};
	virtual ~Model() {};

	PFunCreateModel       create;
	PFunOpenModel         open;
	PFunSaveModel         save;
	PFunCloseModel        close;
	PFunHasModel          hasModel;
	PFunIsModelModify     isModify;
	PFunBuildModel        build;
	PFunRunModel          run;
	PFunStopModel         stop;
	PFunIsRunning         isRunning;
	PFunGetRuntimeMode    getRuntimeMode;
	PFunSetRuntimeMode    setRuntimeMode;
	PFunGetModelStructure getStructure;
	PFunReadFile          read;
	PFunWriteFile         write;

	ModelAction           action;
};

// ----------------------------------------------------------------------------
// ---------- Frame
// ----------------------------------------------------------------------------
// Класс анимации: позволяет переключаться между кадрами анимации.
//
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

// ----------------------------------------------------------------------------
// ---------- Plugin
// ----------------------------------------------------------------------------
// Класс плагина: позволяет остановить плагин и проверить его состояние.
//
typedef void (*PFunPluginStop)( const HMODULE );
typedef bool (*PFunPluginIsStoped)( const HMODULE );

class Plugin {
public:
	Plugin(): stop( NULL ), isStoped( NULL ), hInstance( NULL )  {};
	virtual ~Plugin() {};

	PFunPluginStop     stop;
	PFunPluginIsStoped isStoped;

	HINSTANCE          hInstance;
};

// ----------------------------------------------------------------------------
// ---------- Studio
// ----------------------------------------------------------------------------
// Основной класс при работе с плагином. Объект данного класса передается плагину при его запуске.
// Содержит в себе объекты для управления моделью и анимацией.
// Позволяет скрыть главное окно студии и получить его указатель.
//
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

	PFunShow          show;
	PFunIsShow        isShow;
	PFunGetMainFrame  mainFrame;
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
	PM_MODEL_RUNTIMEMODE,
	PM_STUDIO_SHOWCHANGED              // param = int:Win32API::ShowWindow::nCmdShow
};

typedef void (*PFunGetPluginInfo)( PluginInfo* );
typedef bool (*PFunStartPlugin)( const Studio& studio );
typedef void (*PFunAfterStartPlugin)();
typedef void (*PFunStopPlugin)();
typedef const int (*PFunEnumMessages)();
typedef void (*PFunPluginProc)( const int, void* );
typedef void (*PFunTrace)( const char* );
typedef void (*PFunResults)( const char* );

} // namespace rdoPlugin

extern "C" {
	RDOPLUGIN_DLL void getPluginInfo( rdoPlugin::PluginInfo* info );
	RDOPLUGIN_DLL bool startPlugin( const rdoPlugin::Studio& studio );
	RDOPLUGIN_DLL void afterStartPlugin();
	RDOPLUGIN_DLL void stopPlugin();
	RDOPLUGIN_DLL const int enumMessages();
	RDOPLUGIN_DLL void pluginProc( const int message, void* param1 );
	RDOPLUGIN_DLL void trace( const char* line );
	RDOPLUGIN_DLL void results( const char* lines );
}

#endif // RDOPLUGIN_H
