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
	std::string name;
	int version_major;
	int version_minor;
	int version_build;
	std::string version_info;
	std::string description;

public:
	RDOStudioPlugin();
	virtual ~RDOStudioPlugin();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
class RDOStudioPlugins
{
private:
	std::vector< RDOStudioPlugin* > list;
	void enumPlugins( const std::string& mask );

public:
	RDOStudioPlugins();
	virtual ~RDOStudioPlugins();

	void init();
};

// ----------------------------------------------------------------------------
extern RDOStudioPlugins plugins;

#endif // RDOSTUDIOPLUGINS_H
