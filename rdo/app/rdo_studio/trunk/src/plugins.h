/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      plugins.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.10.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_PLUGINS_H_
#define _RDO_STUDIO_MFC_PLUGINS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/plugins/common/rdoplugin.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioPlugin
// --------------------------------------------------------------------------------
class RDOStudioPlugin
{
friend class RDOStudioPlugins;

private:
	std::string modulName;
	HMODULE     lib;
	std::string name;
	int version_major;
	int version_minor;
	int version_build;
	std::string version_info;
	std::string description;
	rdoPlugin::PluginState state;
	bool restoreState;
	rdoPlugin::PluginRunMode defaultRunMode;
	rdoPlugin::PluginRunMode runMode;

	rdoPlugin::PFunPluginProc pluginProc;
	rdoPlugin::PFunTrace      trace;
	rdoPlugin::PFunResults    results;

	CMutex mutex;
	bool closed;

	std::string getProfilePath() const;

public:
	RDOStudioPlugin( const std::string& modulName );
	virtual ~RDOStudioPlugin();

	static bool isRDOStudioPlugin( const std::string& modulName );

	std::string getName() const        { return name;          }

	int getVersionMajor() const        { return version_major; }
	int getVersionMinor() const        { return version_minor; }
	int getVersionBuild() const        { return version_build; }
	std::string getVersionInfo() const { return version_info;  }

	std::string getDescription() const { return description;   }

	rdoPlugin::PluginState getState() const     { return state; }
	void setState( const rdoPlugin::PluginState value );

	bool getRestoreState() const                { return restoreState; }
	void setRestoreState( const bool value );

	rdoPlugin::PluginRunMode getRunMode() const { return runMode; }
	void setRunMode( const rdoPlugin::PluginRunMode value );
	rdoPlugin::PluginRunMode getDefaultRunMode() const { return defaultRunMode; }

	std::string getFileName() const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioPlugins
// --------------------------------------------------------------------------------
static const ruint PLUGIN_MUSTEXIT_MESSAGE = ::RegisterWindowMessage( "PLUGIN_MUSTEXIT_MESSAGE" );

class RDOStudioPlugins
{
friend class RDOStudioPlugin;

private:
	std::vector< RDOStudioPlugin* > list;
	void enumPlugins( const std::string& mask );

	void init();

	rdoPlugin::Studio studio;

	typedef std::multimap< const int, RDOStudioPlugin* > messageList;
	messageList messages;
	void setMessageReflect( const int message, RDOStudioPlugin* plugin );
	void clearMessageReflect( RDOStudioPlugin* plugin );

	std::vector< RDOStudioPlugin* > trace;
	CMutex mutex;
	void setTrace( RDOStudioPlugin* plugin );
	void clearTrace( RDOStudioPlugin* plugin );

	std::vector< RDOStudioPlugin* > results;
	void setResults( RDOStudioPlugin* plugin );
	void clearResults( RDOStudioPlugin* plugin );

	std::string modelStructure;

	std::list< rdoPlugin::ModelActionType > actionDisabled;

	static void __stdcall pluginStop( const HMODULE lib );
	static bool __stdcall pluginIsStoped( const HMODULE lib );

	int lastCmdShow;
	static void __stdcall studioShow( int cmdShow );
	static HWND __stdcall studioGetMainFrame();

	static void __stdcall actionEnable( rdoPlugin::ModelActionType action );
	static void __stdcall actionDisable( rdoPlugin::ModelActionType action );
	static bool __stdcall actionState( rdoPlugin::ModelActionType action );
	static bool __stdcall newModel( const char* modelName, const char* modelPath );
	static bool __stdcall openModel( const char* modelName );
	static bool __stdcall saveModel();
	static bool __stdcall closeModel();
	static bool __stdcall hasModel();
	static bool __stdcall isModelModify();
	static bool __stdcall buildModel();
	static bool __stdcall runModel();
	static bool __stdcall stopModel();
	static bool __stdcall isModelRunning();
	static rdoPlugin::ModelRuntimeMode __stdcall getModelRuntimeMode();
	static void __stdcall setModelRuntimeMode( rdoPlugin::ModelRuntimeMode runtimeMode );
	static const char* __stdcall getModelStructure();
	static bool __stdcall readFile( rdoPlugin::ModelFileType file_type, char** data );
	static bool __stdcall writeFile( rdoPlugin::ModelFileType file_type, const char* data );

	static bool __stdcall isFrameDescribed();
	static double __stdcall getFrameShowRate();
	static void __stdcall setFrameShowRate( double value );
	static void __stdcall showNextFrame();
	static void __stdcall showPrevFrame();
	static bool __stdcall canShowNextFrame();
	static bool __stdcall canShowPrevFrame();
	static int __stdcall getFrameCount();
	static const char* __stdcall getFrameName( int index );
	static void __stdcall showFrame( int index );
	static void __stdcall closeAllFrame();

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	const std::vector< RDOStudioPlugin* >& getList() { return list; }

	void stopPluginByStudio( const HMODULE lib );
	void modelStart();
	void modelStop( bool model_no_error = true );
	void traceProc( const std::string& str );
	void pluginProc( const int message, void* param1 = NULL );
	static bool __stdcall studioIsShow();

	bool canAction( rdoPlugin::ModelActionType action );

	void saveMainFrameState( int cmdShow );

	static int comparePluginsByName( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByVersion( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByRunMode( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByState( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByDescription( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
};

// --------------------------------------------------------------------------------
extern RDOStudioPlugins* plugins;

#endif // _RDO_STUDIO_MFC_PLUGINS_H_
