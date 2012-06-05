#include <vcl.h>
#include <windows.h>
#include <string>
#pragma hdrstop

// some info about options (important):
// 1. Project/Options/Application/Target file extension = rsp
// 2. Project/Options/Compiler/Treat enum types as ints = yes
// 3. Project/Options/Advanced Compiler/Calling convention = "C" (default value)
// 4. Project/Options/Directories/Include path: insert "..\rdo_plugin" path
//
// not important options:
// 5. Project/Options/Packages/Build with runtime packages = no (for running on PC without VCL)
// 6. Project/Options/Linker/Generate import library = no (*.lib file is not needed for plugin)
// 7. Project/Options/Directories/Final output: setup to $(path of RAO-studio.exe)\plugins path
// 8. Run/Parameters/Local/Host Application: setup to $(path + file name of RAO-studio.exe)
//
// don't forget to include for project (Project/Add to Project) "rdo_plugin_cbuilder.def" file

// include plugin header file with RDOPLUGIN_EXPORTS definition for export functions
#define RDOPLUGIN_EXPORTS
#include <rdoplugin.h>

#pragma argsused

#include "rdoplugincbuilderform.h"

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

void getPluginInfo( rdoPlugin::PluginInfo* info )
{
	static char* name         = "CBuilder Plugin";
	static char* version_info = "";
	static char* description  = "CBuilder-Based Plugin";
	info->name = name;
	info->version_major  = 1;
	info->version_minor  = 0;
	info->version_build  = 1;
	info->version_info   = version_info;
	info->description    = description;
	info->defaultRunMode = rdoPlugin::prmNoAuto;
}

bool startPlugin( const rdoPlugin::Studio* studio )
{
	CBuilderPluginForm = new TCBuilderPluginForm( static_cast<TComponent*>(NULL), studio );
	CBuilderPluginForm->Show();
	return true;
}

void stopPlugin()
{
	if ( CBuilderPluginForm ) {
		delete CBuilderPluginForm;
		CBuilderPluginForm = NULL;
	}
}

const int enumMessages()
{
	static int i = 0;
	int messages[] = { rdoPlugin::PM_MODEL_NEW, rdoPlugin::PM_MODEL_OPEN, rdoPlugin::PM_MODEL_SAVE, rdoPlugin::PM_MODEL_CLOSE, rdoPlugin::PM_MODEL_NAME_CHANGED, rdoPlugin::PM_MODEL_MODIFY, rdoPlugin::PM_MODEL_BUILD_OK, rdoPlugin::PM_MODEL_BUILD_FAILD, rdoPlugin::PM_MODEL_BEFORE_START, rdoPlugin::PM_MODEL_AFTER_START, rdoPlugin::PM_MODEL_FINISHED, rdoPlugin::PM_MODEL_STOP_CANCEL, rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR, rdoPlugin::PM_MODEL_RUNTIMEMODE };
	static const int cnt = sizeof( messages )/sizeof( int );
	if ( i < cnt ) {
		return messages[i++];
	}
	return 0;
}

void pluginProc( const int message )
{
	if ( message == rdoPlugin::PM_MODEL_NEW ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_NEW" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_OPEN ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_OPEN" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_SAVE ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_SAVE" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_CLOSE ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_CLOSE" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_NAME_CHANGED ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_NAME_CHANGED" );
	} else if ( message == rdoPlugin::PM_MODEL_MODIFY ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_MODIFY" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_BUILD_OK ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_BUILD_OK" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_BUILD_FAILD ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_BUILD_FAILD" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_BEFORE_START ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_BEFORE_START" );
		CBuilderPluginForm->updateControlsState();
		std::string str = CBuilderPluginForm->studio.model.getStructure();
		std::string::size_type pos = str.find( '\n' );
		if ( pos != std::string::npos ) {
			while ( pos != std::string::npos ) {
				std::string str2( str.begin(), 0, pos );
				CBuilderPluginForm->insertLine( str2.c_str() );
				str.erase( 0, pos + 1 );
				pos = str.find( '\n' );
			}
			std::string str2( str.begin(), 0, pos );
			if ( !str2.empty() ) {
				CBuilderPluginForm->insertLine( str2.c_str() );
			}
		} else {
			CBuilderPluginForm->insertLine( str.c_str() );
		}
	} else if ( message == rdoPlugin::PM_MODEL_AFTER_START ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_AFTER_START" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_FINISHED ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_FINISHED" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_STOP_CANCEL ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_STOP_CANCEL" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_STOP_RUNTIME_ERROR" );
		CBuilderPluginForm->updateControlsState();
	} else if ( message == rdoPlugin::PM_MODEL_RUNTIMEMODE ) {
		CBuilderPluginForm->insertLine( "PM_MODEL_RUNTIMEMODE" );
		CBuilderPluginForm->updateControlsState();
	}
}

void trace( const char* line )
{
	CBuilderPluginForm->insertLine( line );
}

void results( const char* lines )
{
	std::string str = lines;
	std::string::size_type pos = str.find( '\n' );
	if ( pos != std::string::npos ) {
		while ( pos != std::string::npos ) {
			std::string str2( str.begin(), 0, pos );
			CBuilderPluginForm->insertLine( str2.c_str() );
			str.erase( 0, pos + 1 );
			pos = str.find( '\n' );
		}
		std::string str2( str.begin(), 0, pos );
		if ( !str2.empty() ) {
			CBuilderPluginForm->insertLine( str2.c_str() );
		}
	} else {
		CBuilderPluginForm->insertLine( str.c_str() );
	}
}

