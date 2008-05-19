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

typedef void (__stdcall *PFunModelActionEnable)( ModelActionType action );
typedef void (__stdcall *PFunModelActionDisable)( ModelActionType action );
typedef bool (__stdcall *PFunModelActionState)( ModelActionType action );

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
	MRTM_Pause,
	MRTM_BreakPoint
};

enum ModelFileType { PAT = 0, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC };

typedef bool (__stdcall *PFunCreateModel)( const char* modelName, const char* modelPath );
typedef bool (__stdcall *PFunOpenModel)( const char* modelName );
typedef bool (__stdcall *PFunSaveModel)();
typedef bool (__stdcall *PFunCloseModel)();
typedef bool (__stdcall *PFunHasModel)();
typedef bool (__stdcall *PFunIsModelModify)();
typedef bool (__stdcall *PFunBuildModel)();
typedef bool (__stdcall *PFunRunModel)();
typedef bool (__stdcall *PFunStopModel)();
typedef bool (__stdcall *PFunIsRunning)();
typedef ModelRuntimeMode (__stdcall *PFunGetRuntimeMode)();
typedef void (__stdcall *PFunSetRuntimeMode)( ModelRuntimeMode runtimeMode );
typedef const char* (__stdcall *PFunGetModelStructure)();
typedef bool (__stdcall *PFunReadFile)( ModelFileType file_type, char** data );
typedef bool (__stdcall *PFunWriteFile)( ModelFileType file_type, const char* data );

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
typedef bool (__stdcall *PFunIsFrameDescribed)();
typedef double (__stdcall *PFunGetShowRate)();
typedef void (__stdcall *PFunSetShowRate)( double value );
typedef void (__stdcall *PFunShowNextFrame)();
typedef void (__stdcall *PFunShowPrevFrame)();
typedef bool (__stdcall *PFunCanShowNextFrame)();
typedef bool (__stdcall *PFunCanShowPrevFrame)();
typedef int (__stdcall *PFunGetFrameCount)();
typedef const char* (__stdcall *PFunGetFrameName)( int index );
typedef void (__stdcall *PFunShowFrame)( int index );
typedef void (__stdcall *PFunCloseAllFrame)();

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
typedef void (__stdcall *PFunPluginStop)( const HMODULE );
typedef bool (__stdcall *PFunPluginIsStoped)( const HMODULE );

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
typedef void (__stdcall *PFunShow)( int cmdShow );
typedef bool (__stdcall *PFunIsShow)();
typedef HWND (__stdcall *PFunGetMainFrame)();

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

typedef void (__stdcall *PFunGetPluginInfo)( PluginInfo* );
typedef bool (__stdcall *PFunStartPlugin)( const Studio& studio );
typedef void (__stdcall *PFunAfterStartPlugin)();
typedef void (__stdcall *PFunStopPlugin)();
typedef const int (__stdcall *PFunEnumMessages)();
typedef void (__stdcall *PFunPluginProc)( const int, void* );
typedef void (__stdcall *PFunTrace)( const char* );
typedef void (__stdcall *PFunResults)( const char* );

} // namespace rdoPlugin

extern "C" {
	void RDOPLUGIN_DLL __stdcall getPluginInfo( rdoPlugin::PluginInfo* info );
	bool RDOPLUGIN_DLL __stdcall startPlugin( const rdoPlugin::Studio& studio );
	void RDOPLUGIN_DLL __stdcall afterStartPlugin();
	void RDOPLUGIN_DLL __stdcall stopPlugin();
	const int RDOPLUGIN_DLL __stdcall enumMessages();
	void RDOPLUGIN_DLL __stdcall pluginProc( const int message, void* param1 );
	void RDOPLUGIN_DLL __stdcall trace( const char* line );
	void RDOPLUGIN_DLL __stdcall results( const char* lines );
}

#endif // RDOPLUGIN_H
