#include "stdafx.h"
#include "rdostudioplugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugin
// ----------------------------------------------------------------------------
RDOStudioPlugin::RDOStudioPlugin()
{
	name = "";
	version_major = 0;
	version_minor = 0;
	version_build = 0;
	version_info  = "";
	description   = "";
}

RDOStudioPlugin::~RDOStudioPlugin()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
RDOStudioPlugins plugins;

RDOStudioPlugins::RDOStudioPlugins()
{
}

RDOStudioPlugins::~RDOStudioPlugins()
{
	std::vector< RDOStudioPlugin* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

void RDOStudioPlugins::enumPlugins( const std::string& mask )
{
	CFileFind finder;
	if ( finder.FindFile( mask.c_str() ) ) {
		BOOL flag = TRUE;
		while ( flag ) {
			flag = finder.FindNextFile();
			if ( finder.IsDirectory() && !finder.IsDots() ) {
				std::string subDir = finder.GetFilePath();
				subDir += "\\*.*";
				enumPlugins( subDir );
			} else {
				std::string fullname = finder.GetFilePath();
				if ( fullname.rfind( ".rsp" ) == fullname.length() - 4 ) {
					HMODULE lib = ::LoadLibrary( finder.GetFilePath() );
					if ( lib ) {
						PFunGetPluginInfo getPluginInfo = reinterpret_cast<PFunGetPluginInfo>(::GetProcAddress( lib, "getPluginInfo" ));
						if ( getPluginInfo ) {
							PluginInfo info;
							getPluginInfo( &info );
							RDOStudioPlugin* plugin = new RDOStudioPlugin();
							plugin->name          = info.name;
							plugin->version_major = info.version_major;
							plugin->version_minor = info.version_minor;
							plugin->version_build = info.version_build;
							plugin->version_info  = info.version_info;
							plugin->description   = info.description;
							list.push_back( plugin );
						}
						::FreeLibrary( lib );
					}
				}
			}
		}
	}
	finder.Close();
}

void RDOStudioPlugins::init()
{
	std::string path = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		path = szExeName;
	}
	if ( !path.empty() ) {
		enumPlugins( path + "*.*" );
	}
	int i = 1;
	std::vector< RDOStudioPlugin* >::iterator it = list.begin();
	while ( it != list.end() ) {
		RDOStudioPlugin* plugin = *it;
		TRACE( "%d. plugin name = %s, version %d.%d, build %d, version info = %s, description = %s\r\n", i++, plugin->name.c_str(), plugin->version_major, plugin->version_minor, plugin->version_build, plugin->version_info.c_str(), plugin->description.c_str() );
		it++;
	}
}
