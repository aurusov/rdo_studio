#ifndef RDOSTUDIOPLUGINS_H
#define RDOSTUDIOPLUGINS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoplugin.h>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugin
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
static const int PLUGIN_MUSTEXIT_MESSAGE = ::RegisterWindowMessage( "PLUGIN_MUSTEXIT_MESSAGE" );
//static const int PLUGIN_STARTMODEL_MESSAGE = ::RegisterWindowMessage( "PLUGIN_START_MODEL_MESSAGE" );
//static const int PLUGIN_STOPMODEL_MESSAGE  = ::RegisterWindowMessage( "PLUGIN_STOP_MODEL_MESSAGE" );

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

	static void pluginStop( const HMODULE lib );
	static bool pluginIsStoped( const HMODULE lib );

	int lastCmdShow;
	static void studioShow( int cmdShow );
	static HWND studioGetMainFrame();

	static void newModel();
	static bool openModel( const char* modelName );
	static void saveModel();
	static void closeModel();
	static bool hasModel();
	static bool isModelModify();
	static void buildModel();
	static void runModel();
	static void stopModel();
	static bool isModelRunning();
	static rdoPlugin::ModelRuntimeMode getModelRuntimeMode();
	static void setModelRuntimeMode( rdoPlugin::ModelRuntimeMode runtimeMode );
	static const char* getModelStructure();

	static bool isFrameDescribed();
	static double getFrameShowRate();
	static void setFrameShowRate( double value );
	static void showNextFrame();
	static void showPrevFrame();
	static bool canShowNextFrame();
	static bool canShowPrevFrame();
	static int getFrameCount();
	static const char* getFrameName( int index );
	static void showFrame( int index );
	static void closeAllFrame();

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	const std::vector< RDOStudioPlugin* >& getList() { return list; }

	void stopPluginByStudio( const HMODULE lib );
	void modelStart();
	void modelStop( bool model_no_error = true );
	void traceProc( const std::string& str );
	void pluginProc( const int message, void* param1 = NULL );
	static bool studioIsShow();

	void saveMainFrameState( int cmdShow );

	static int comparePluginsByName( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByVersion( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByRunMode( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByState( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByDescription( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
};

// ----------------------------------------------------------------------------
extern RDOStudioPlugins* plugins;

#endif // RDOSTUDIOPLUGINS_H
