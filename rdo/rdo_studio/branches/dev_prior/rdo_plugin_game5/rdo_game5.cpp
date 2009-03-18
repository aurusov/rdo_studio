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
// don't forget to include for project (Project/Add to Project) "rdo_plugin_game5.def" file

// include plugin header file with RDOPLUGIN_EXPORTS definition for export functions
#define RDOPLUGIN_EXPORTS
#ifndef _EXPLICTDLL_
#define _EXPLICTDLL_
#include <rdoplugin.h>

#include "rdo_plugin_game5_mainform.h"

HINSTANCE hInstance = NULL;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	hInstance = hinst;
	return 1;
}

void __stdcall getPluginInfo( rdoPlugin::PluginInfo* info )
{
	static char* name         = "Игра Пять";
	static char* version_info = "";
	static char* description  = "Пример задачи поиска на графе пространства состояний. Усеченные пятнашки.";
	info->name = name;
	info->version_major  = 1;
	info->version_minor  = 0;
	info->version_build  = 1;
	info->version_info   = version_info;
	info->description    = description;
	info->defaultRunMode = rdoPlugin::prmNoAuto;
}

bool __stdcall startPlugin( const rdoPlugin::Studio& studio )
{
	MainForm = new TMainForm( static_cast<TComponent*>(NULL), studio, hInstance );
	MainForm->Show();
	return true;
}

void __stdcall afterStartPlugin()
{
//	MainForm->studio.model.action.disable( rdoPlugin::maCreate );
//	MainForm->studio.model.action.disable( rdoPlugin::maOpen );
//	MainForm->studio.model.action.disable( rdoPlugin::maSave );
//	MainForm->studio.model.action.disable( rdoPlugin::maClose );
//	MainForm->studio.model.action.disable( rdoPlugin::maBuild );
//	MainForm->studio.model.action.disable( rdoPlugin::maRun );
}

void __stdcall stopPlugin()
{
	if ( MainForm ) {
		TMainForm* form = MainForm;
		MainForm = NULL;
		delete form;
	}
}

const int __stdcall enumMessages()
{
	static int i = 0;
	int messages[] = { rdoPlugin::PM_MODEL_NEW, rdoPlugin::PM_MODEL_OPEN, rdoPlugin::PM_MODEL_SAVE, rdoPlugin::PM_MODEL_CLOSE, rdoPlugin::PM_MODEL_NAME_CHANGED, rdoPlugin::PM_MODEL_MODIFY, rdoPlugin::PM_MODEL_BUILD_OK, rdoPlugin::PM_MODEL_BUILD_FAILD, rdoPlugin::PM_MODEL_BEFORE_START, rdoPlugin::PM_MODEL_AFTER_START, rdoPlugin::PM_MODEL_FINISHED, rdoPlugin::PM_MODEL_STOP_CANCEL, rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR, rdoPlugin::PM_MODEL_RUNTIMEMODE, rdoPlugin::PM_STUDIO_SHOWCHANGED };
	static const int cnt = sizeof( messages )/sizeof( int );
	if ( i < cnt ) {
		return messages[i++];
	}
	return 0;
}

void __stdcall pluginProc( const int message, void* param1 )
{
	switch ( message ) {
    	case rdoPlugin::PM_STUDIO_SHOWCHANGED: {
        	if ( MainForm ) {
                if ( !MainForm->studio.isShow() ) {
                    MainForm->Source->Caption = "Показать исходники";
                } else {
                    MainForm->Source->Caption = "Скрыть исходники";
                }
            }
        	break;
        }
    }
}

void __stdcall trace( const char* line )
{
//	CBuilderPluginForm->insertLine( line );
}

void __stdcall results( const char* lines )
{
/*
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
*/
}

#endif // _EXPLICTDLL_

