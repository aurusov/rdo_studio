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
	tstring     modulName;
	HMODULE     lib;
	tstring     name;
	int version_major;
	int version_minor;
	int version_build;
	tstring     version_info;
	tstring     description;
	rdoPlugin::PluginState state;
	rbool restoreState;
	rdoPlugin::PluginRunMode defaultRunMode;
	rdoPlugin::PluginRunMode runMode;

	rdoPlugin::PFunPluginProc pluginProc;
	rdoPlugin::PFunTrace      trace;
	rdoPlugin::PFunResults    results;

	CMutex mutex;
	rbool closed;

	tstring getProfilePath() const;

public:
	RDOStudioPlugin( CREF(tstring) modulName );
	virtual ~RDOStudioPlugin();

	static rbool isRDOStudioPlugin( CREF(tstring) modulName );

	tstring getName() const        { return name;          }

	int     getVersionMajor() const { return version_major; }
	int     getVersionMinor() const { return version_minor; }
	int     getVersionBuild() const { return version_build; }
	tstring getVersionInfo () const { return version_info;  }

	tstring getDescription () const { return description;   }

	rdoPlugin::PluginState getState() const     { return state; }
	void setState( const rdoPlugin::PluginState value );

	rbool getRestoreState() const               { return restoreState; }
	void  setRestoreState( const rbool value );

	rdoPlugin::PluginRunMode getRunMode() const { return runMode; }
	void setRunMode( const rdoPlugin::PluginRunMode value );
	rdoPlugin::PluginRunMode getDefaultRunMode() const { return defaultRunMode; }

	tstring getFileName() const;
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
	void enumPlugins( CREF(tstring) mask );

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

	tstring modelStructure;

	std::list< rdoPlugin::ModelActionType > actionDisabled;

	static void  __stdcall pluginStop( const HMODULE lib );
	static rbool __stdcall pluginIsStoped( const HMODULE lib );

	int lastCmdShow;
	static void __stdcall studioShow( int cmdShow );
	static HWND __stdcall studioGetMainFrame();

	static void  __stdcall actionEnable( rdoPlugin::ModelActionType action );
	static void  __stdcall actionDisable( rdoPlugin::ModelActionType action );
	static rbool __stdcall actionState( rdoPlugin::ModelActionType action );
	static rbool __stdcall newModel( const char* modelName, const char* modelPath );
	static rbool __stdcall openModel( const char* modelName );
	static rbool __stdcall saveModel();
	static rbool __stdcall closeModel();
	static rbool __stdcall hasModel();
	static rbool __stdcall isModelModify();
	static rbool __stdcall buildModel();
	static rbool __stdcall runModel();
	static rbool __stdcall stopModel();
	static rbool __stdcall isModelRunning();
	static rdoPlugin::ModelRuntimeMode __stdcall getModelRuntimeMode();
	static void __stdcall setModelRuntimeMode( rdoPlugin::ModelRuntimeMode runtimeMode );
	static const char* __stdcall getModelStructure();
	static rbool __stdcall readFile( rdoPlugin::ModelFileType file_type, char** data );
	static rbool __stdcall writeFile( rdoPlugin::ModelFileType file_type, const char* data );

	static rbool __stdcall isFrameDescribed();
	static double __stdcall getFrameShowRate();
	static void  __stdcall setFrameShowRate( double value );
	static void  __stdcall showNextFrame();
	static void  __stdcall showPrevFrame();
	static rbool __stdcall canShowNextFrame();
	static rbool __stdcall canShowPrevFrame();
	static int   __stdcall getFrameCount();
	static const char* __stdcall getFrameName( int index );
	static void  __stdcall showFrame( int index );
	static void  __stdcall closeAllFrame();

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	const std::vector< RDOStudioPlugin* >& getList() { return list; }

	void stopPluginByStudio( const HMODULE lib );
	void modelStart();
	void modelStop( rbool model_no_error = true );
	void traceProc( CREF(tstring) str );
	void pluginProc( const int message, void* param1 = NULL );
	static rbool __stdcall studioIsShow();

	rbool canAction( rdoPlugin::ModelActionType action );

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
