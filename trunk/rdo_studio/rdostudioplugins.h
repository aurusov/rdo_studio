#ifndef RDOSTUDIOPLUGINS_H
#define RDOSTUDIOPLUGINS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoplugin.h>
#include <string>
#include <map>

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugin
// ----------------------------------------------------------------------------
class RDOStudioPlugin
{
friend class RDOStudioPlugins;

private:
	std::string name;
	int version_major;
	int version_minor;
	int version_build;
	std::string version_info;
	std::string description;

public:
	RDOStudioPlugin();
	virtual ~RDOStudioPlugin();

	std::string getName() const { return name; }
	int getVersionMajor() const { return version_major; }
	int getVersionMinor() const { return version_minor; }
	int getVersionBuild() const { return version_build; }
	std::string getVersionInfo() const { return version_info; }
	std::string getDescription() const { return description; }
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
class RDOStudioPlugins
{
private:
	std::multimap< std::string, RDOStudioPlugin* > list;
	void enumPlugins( const std::string& mask );

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	void init();

	const std::multimap< std::string, RDOStudioPlugin* >& getList() { return list; }
};

// ----------------------------------------------------------------------------
extern RDOStudioPlugins plugins;

#endif // RDOSTUDIOPLUGINS_H
