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
	rdoPlugin::PluginRunMode runMode;

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
	rdoPlugin::PluginRunMode getDefaultRunMode() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
static const int PLUGIN_STARTMODEL_MESSAGE = ::RegisterWindowMessage( "PLUGIN_START_MODEL_MESSAGE" );
static const int PLUGIN_STOPMODEL_MESSAGE  = ::RegisterWindowMessage( "PLUGIN_STOP_MODEL_MESSAGE" );

class RDOStudioPlugins
{
private:
	std::vector< RDOStudioPlugin* > list;
	void enumPlugins( const std::string& mask );

	void init();

	static void modelStartNotify();
	static void modelStopNotify();

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	const std::vector< RDOStudioPlugin* >& getList() { return list; }

	void modelStart();
	void modelStop();

	void stopPlugin( const HMODULE lib ) const;

	static int comparePluginsByName( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByVersion( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByRunMode( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByState( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
	static int comparePluginsByDescription( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 );
};

// ----------------------------------------------------------------------------
extern RDOStudioPlugins* plugins;

#endif // RDOSTUDIOPLUGINS_H
