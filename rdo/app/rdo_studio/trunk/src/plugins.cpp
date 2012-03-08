/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      plugins.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.10.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/plugins.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioPlugin
// --------------------------------------------------------------------------------
RDOStudioPlugin::RDOStudioPlugin( CREF(tstring) _modulName ):
	modulName( _modulName ),
	lib( NULL ),
	name( "" ),
	version_major( 0 ),
	version_minor( 0 ),
	version_build( 0 ),
	version_info( "" ),
	description( "" ),
	state( rdoPlugin::psStoped ),
	restoreState( true ),
	defaultRunMode( rdoPlugin::prmNoAuto ),
	runMode( rdoPlugin::prmNoAuto ),
	pluginProc( NULL ),
	trace( NULL ),
	results( NULL ),
	closed( false )
{
	lib = ::LoadLibrary( modulName.c_str() );
	if ( lib ) {
		rdoPlugin::PFunGetPluginInfo getPluginInfo = reinterpret_cast<rdoPlugin::PFunGetPluginInfo>(::GetProcAddress( lib, "getPluginInfo" ));
		if ( getPluginInfo ) {
			rdoPlugin::PluginInfo info;
			getPluginInfo( &info );
			name           = info.name;
			version_major  = info.version_major;
			version_minor  = info.version_minor;
			version_build  = info.version_build;
			version_info   = info.version_info;
			description    = info.description;
			defaultRunMode = info.defaultRunMode;
		}
		runMode = getDefaultRunMode();
	}
	runMode      = static_cast<rdoPlugin::PluginRunMode>(AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "runMode", runMode ));
	restoreState = AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "restoreState", restoreState ) ? true : false;
	if ( studioApp.isPluginAutoStart( this ) ) {
		setState( rdoPlugin::psActive );
	} else {
		if ( restoreState ) {
			setState( static_cast<rdoPlugin::PluginState>(AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "state", state )) );
		}
		if ( runMode == rdoPlugin::prmStudioStartUp ) {
			setState( rdoPlugin::psActive );
		} else if ( getState() == rdoPlugin::psStoped && lib ) {
			::FreeLibrary( lib );
			lib = NULL;
		}
	}
}

RDOStudioPlugin::~RDOStudioPlugin()
{
	restoreState = false;
	setState( rdoPlugin::psStoped );
}

bool RDOStudioPlugin::isRDOStudioPlugin( CREF(tstring) modulName )
{
	UNUSED(modulName);
	return true;

	//bool res = false;
	//HMODULE local_lib = ::LoadLibrary( modulName.c_str() );
	//if ( local_lib ) {
	//	rdoPlugin::PFunGetPluginInfo getPluginInfo = reinterpret_cast<rdoPlugin::PFunGetPluginInfo>(::GetProcAddress( local_lib, "getPluginInfo" ));
	//	if ( getPluginInfo ) {
	//		res = true;
	//	}
	//	::FreeLibrary( local_lib );
	//}
	//return res;
}

tstring RDOStudioPlugin::getProfilePath() const
{
	return rdo::format( "plugins\\%s_%d.%d.%d", name.c_str(), version_major, version_minor, version_build );
}

void RDOStudioPlugin::setState( const rdoPlugin::PluginState value )
{
	mutex.Lock();
	if ( state != value ) {
		state = value;
		if ( state == rdoPlugin::psActive ) {
			if ( !lib ) {
				lib = ::LoadLibrary( modulName.c_str() );
				if ( !lib ) {
					state = rdoPlugin::psStoped;
				}
			}
			if ( state == rdoPlugin::psActive ) {
				rdoPlugin::PFunStartPlugin startPlugin = reinterpret_cast<rdoPlugin::PFunStartPlugin>(::GetProcAddress( lib, "startPlugin" ));
				plugins->studio.plugin.hInstance = lib;
				if ( startPlugin && startPlugin( plugins->studio ) ) {
					plugins->mutex.Lock();
					// setup messages procedure
					rdoPlugin::PFunEnumMessages enumMessages = reinterpret_cast<rdoPlugin::PFunEnumMessages>(::GetProcAddress( lib, "enumMessages" ));
					if ( enumMessages ) {
						pluginProc = reinterpret_cast<rdoPlugin::PFunPluginProc>(::GetProcAddress( lib, "pluginProc" ));
						if ( pluginProc ) {
							while ( const int message = enumMessages() ) {
								plugins->setMessageReflect( message, this );
							}
						}
					} else {
						pluginProc = NULL;
					}
					// setup trace procedure
					trace = reinterpret_cast<rdoPlugin::PFunTrace>(::GetProcAddress( lib, "trace" ));
					if ( trace ) {
						plugins->setTrace( this );
					}
					// setup results procedure
					results = reinterpret_cast<rdoPlugin::PFunResults>(::GetProcAddress( lib, "results" ));
					if ( results ) {
						plugins->setResults( this );
					}
					plugins->mutex.Unlock();
					rdoPlugin::PFunAfterStartPlugin afterStartPlugin = reinterpret_cast<rdoPlugin::PFunAfterStartPlugin>(::GetProcAddress( lib, "afterStartPlugin" ));
					if ( afterStartPlugin ) {
						afterStartPlugin();
					}
				} else {
					state = rdoPlugin::psStoped;
				}
				closed = false;
			}
		}
		if ( state == rdoPlugin::psStoped ) {
			if ( lib ) {
				TRACE( "plugin stoped begin\n" );
				plugins->mutex.Lock();
				closed = true;
				plugins->clearMessageReflect( this );
				pluginProc = NULL;
				plugins->clearTrace( this );
				trace = NULL;
				plugins->clearResults( this );
				results = NULL;
				rdoPlugin::PFunStopPlugin stopPlugin = reinterpret_cast<rdoPlugin::PFunStopPlugin>(::GetProcAddress( lib, "stopPlugin" ));
				if ( stopPlugin ) {
					stopPlugin();
				}
				::FreeLibrary( lib );
				lib = NULL;
				plugins->mutex.Unlock();
				TRACE( "plugin stoped end\n" );
			}
		}
		if ( getRestoreState() ) {
			AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "state", state );
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugin::setRestoreState( const bool value )
{
	if ( restoreState != value ) {
		restoreState = value;
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "restoreState", restoreState );
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "state", state );
	}
}

void RDOStudioPlugin::setRunMode( const rdoPlugin::PluginRunMode value )
{
	if ( runMode != value ) {
		runMode = value;
		if ( runMode == rdoPlugin::prmModelStartUp && !model->isRunning() ) {
			setState( rdoPlugin::psStoped );
		}
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "runMode", runMode );
	}
}

tstring RDOStudioPlugin::getFileName() const
{
	tstring name = modulName;
	tstring::size_type pos = name.find_last_of( '.' );
	if ( pos != tstring::npos ) {
		tstring s;
		s.assign( &name[0], pos );
		name = s;
	}
	static char szDelims[] = " \t\n\r";
	pos = name.find_last_of( '\\' );
	if ( pos == tstring::npos ) {
		pos = name.find_last_of( '/' );
	}
	if ( pos != tstring::npos ) {
		tstring s( name, pos + 1, name.length() - pos );
		name = s;
	}
	return name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioPlugins
// --------------------------------------------------------------------------------
RDOStudioPlugins* plugins;

RDOStudioPlugins::RDOStudioPlugins():
	modelStructure( "" ),
	lastCmdShow( SW_NORMAL )
{
	studio.show                 = RDOStudioPlugins::studioShow;
	studio.isShow               = RDOStudioPlugins::studioIsShow;
	studio.mainFrame            = RDOStudioPlugins::studioGetMainFrame;

	studio.plugin.stop          = RDOStudioPlugins::pluginStop;
	studio.plugin.isStoped      = RDOStudioPlugins::pluginIsStoped;

	studio.model.create         = RDOStudioPlugins::newModel;
	studio.model.open           = RDOStudioPlugins::openModel;
	studio.model.save           = RDOStudioPlugins::saveModel;
	studio.model.close          = RDOStudioPlugins::closeModel;
	studio.model.hasModel       = RDOStudioPlugins::hasModel;
	studio.model.isModify       = RDOStudioPlugins::isModelModify;
	studio.model.build          = RDOStudioPlugins::buildModel;
	studio.model.run            = RDOStudioPlugins::runModel;
	studio.model.stop           = RDOStudioPlugins::stopModel;
	studio.model.isRunning      = RDOStudioPlugins::isModelRunning;
	studio.model.getRuntimeMode = RDOStudioPlugins::getModelRuntimeMode;
	studio.model.setRuntimeMode = RDOStudioPlugins::setModelRuntimeMode;
	studio.model.getStructure   = RDOStudioPlugins::getModelStructure;
	studio.model.read           = RDOStudioPlugins::readFile;
	studio.model.write          = RDOStudioPlugins::writeFile;
	studio.model.action.enable  = RDOStudioPlugins::actionEnable;
	studio.model.action.disable = RDOStudioPlugins::actionDisable;
	studio.model.action.state   = RDOStudioPlugins::actionState;

	studio.frame.isDescribed   = RDOStudioPlugins::isFrameDescribed;
	studio.frame.getShowRate   = RDOStudioPlugins::getFrameShowRate;
	studio.frame.setShowRate   = RDOStudioPlugins::setFrameShowRate;
	studio.frame.showNext      = RDOStudioPlugins::showNextFrame;
	studio.frame.showPrev      = RDOStudioPlugins::showPrevFrame;
	studio.frame.canShowNext   = RDOStudioPlugins::canShowNextFrame;
	studio.frame.canShowPrev   = RDOStudioPlugins::canShowPrevFrame;
	studio.frame.getCount      = RDOStudioPlugins::getFrameCount;
	studio.frame.getName       = RDOStudioPlugins::getFrameName;
	studio.frame.show          = RDOStudioPlugins::showFrame;
	studio.frame.closeAll      = RDOStudioPlugins::closeAllFrame;

	plugins = this;
	init();
}

RDOStudioPlugins::~RDOStudioPlugins()
{
	mutex.Lock();
	std::vector< RDOStudioPlugin* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
	mutex.Unlock();

	plugins = NULL;
}

void RDOStudioPlugins::enumPlugins( CREF(tstring) mask )
{
	CFileFind finder;
	if ( finder.FindFile( mask.c_str() ) ) {
		BOOL flag = TRUE;
		while ( flag ) {
			flag = finder.FindNextFile();
			if ( finder.IsDirectory() && !finder.IsDots() ) {
				tstring subDir = finder.GetFilePath();
				subDir += "\\*.*";
				enumPlugins( subDir );
			} else if ( !finder.IsDots() ) {
				tstring fullname = finder.GetFilePath();
				if ( fullname.rfind( ".rsp" ) == fullname.length() - 4 ) {
					tstring modulName = finder.GetFilePath();
					if ( RDOStudioPlugin::isRDOStudioPlugin( modulName ) ) {
						RDOStudioPlugin* plugin = new RDOStudioPlugin( modulName );
						mutex.Lock();
						list.push_back( plugin );
						mutex.Unlock();
					}
				}
			}
		}
	}
	finder.Close();
}

void RDOStudioPlugins::init()
{
	tstring path = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		path = rdo::extractFilePath( szExeName );
	}
	if ( !path.empty() ) {
		enumPlugins( path + "\\plugins\\*.*" );
	}
}

void RDOStudioPlugins::setMessageReflect( const int message, RDOStudioPlugin* plugin )
{
	mutex.Lock();
	if ( message && plugin ) {
		bool flag = true;
		messageList::iterator it = messages.find( message );
		while ( it != messages.end() ) {
			if ( (*it).second == plugin ) {
				flag = false;
				break;
			}
			it++;
		}
		if ( flag ) {
			messages.insert( messageList::value_type( message, plugin ) );
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearMessageReflect( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	messageList::iterator it = messages.begin();
	while ( it != messages.end() ) {
		if ( (*it).second == plugin ) {
			it = messages.erase( it );
		} else {
			it++;
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::pluginProc( const int message, void* param1 )
{
	mutex.Lock();
	messageList::iterator it = messages.lower_bound( message );
	while ( it != messages.upper_bound( message ) ) {
		RDOStudioPlugin* plugin = it->second;
		plugin->mutex.Lock();
		if ( !plugin->closed ) {
			plugin->pluginProc( message, param1 );
		}
		plugin->mutex.Unlock();
		it++;
	}
	mutex.Unlock();
}

void RDOStudioPlugins::setTrace( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	if ( plugin && std::find( trace.begin(), trace.end(), plugin ) == trace.end() ) {
		trace.push_back( plugin );
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearTrace( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	std::vector< RDOStudioPlugin* >::iterator it = std::find( trace.begin(), trace.end(), plugin );
	if ( it != trace.end() ) {
		trace.erase( it );
	}
	mutex.Unlock();
}

void RDOStudioPlugins::traceProc( CREF(tstring) str )
{
	plugins->mutex.Lock();
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->trace.begin();
	while ( it != plugins->trace.end() ) {
		RDOStudioPlugin* plugin = *it;
		plugin->mutex.Lock();
		if ( !plugin->closed ) {
			plugin->trace( str.c_str() );
		}
		plugin->mutex.Unlock();
		it++;
	}
	plugins->mutex.Unlock();
}

void RDOStudioPlugins::setResults( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	if ( plugin && std::find( results.begin(), results.end(), plugin ) == results.end() ) {
		results.push_back( plugin );
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearResults( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	std::vector< RDOStudioPlugin* >::iterator it = std::find( results.begin(), results.end(), plugin );
	if ( it != results.end() ) {
		results.erase( it );
	}
	mutex.Unlock();
}

void RDOStudioPlugins::pluginStop( const HMODULE lib )
{
	plugins->mutex.Lock();
	RDOStudioPlugin* plugin = NULL;
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
	while ( it != plugins->list.end() ) {
		if ( (*it)->lib == lib ) {
			plugin = *it;
			break;
		}
		it++;
	}
	if ( plugin ) {
		plugin->mutex.Lock();
		plugin->closed = true;
		AfxGetApp()->PostThreadMessage( PLUGIN_MUSTEXIT_MESSAGE, reinterpret_cast<WPARAM>(lib), 0 );
		plugin->mutex.Unlock();
	}
	plugins->mutex.Unlock();
}

bool RDOStudioPlugins::pluginIsStoped( const HMODULE lib )
{
	plugins->mutex.Lock();
	bool res = false;
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
	while ( it != plugins->list.end() ) {
		RDOStudioPlugin* plugin = *it;
		if ( plugin->lib == lib ) {
			res = plugin->closed;
			break;
		}
		it++;
	}
	plugins->mutex.Unlock();
	return res;
}

void RDOStudioPlugins::studioShow( int cmdShow )
{
	AfxGetApp()->GetMainWnd()->ShowWindow( cmdShow );
	if ( plugins ) plugins->saveMainFrameState( cmdShow );
}

bool RDOStudioPlugins::studioIsShow()
{
	return plugins ? plugins->lastCmdShow == SW_SHOWNORMAL || plugins->lastCmdShow == SW_NORMAL || plugins->lastCmdShow == SW_SHOW || plugins->lastCmdShow == SW_RESTORE || plugins->lastCmdShow == SW_SHOWMAXIMIZED || plugins->lastCmdShow == SW_SHOWNA || plugins->lastCmdShow == SW_SHOWNOACTIVATE : true;
}

void RDOStudioPlugins::saveMainFrameState( int cmdShow )
{
	if ( lastCmdShow != cmdShow ) {
		lastCmdShow = cmdShow;
		pluginProc( rdoPlugin::PM_STUDIO_SHOWCHANGED, reinterpret_cast<void*>(lastCmdShow) );
	}
}

HWND RDOStudioPlugins::studioGetMainFrame()
{
	return AfxGetApp()->GetMainWnd()->m_hWnd;
}

void RDOStudioPlugins::stopPluginByStudio( const HMODULE lib )
{
	RDOStudioPlugin* plugin = NULL;
	if ( plugins && lib ) {
		mutex.Lock();
		std::vector< RDOStudioPlugin* >::const_iterator it = list.begin();
		while ( it != list.end() ) {
			if ( (*it)->lib == lib ) {
				plugin = (*it);
				plugin->setState( rdoPlugin::psStoped );
				break;
			}
			it++;
		}
		mutex.Unlock();
	}
	if ( plugin ) {
		studioApp.autoCloseByPlugin( plugin );
	}
}

void RDOStudioPlugins::actionEnable( rdoPlugin::ModelActionType action )
{
	if ( plugins ) {
		std::list< rdoPlugin::ModelActionType >::iterator it = std::find( plugins->actionDisabled.begin(), plugins->actionDisabled.end(), action );
		if ( it != plugins->actionDisabled.end() ) {
			plugins->actionDisabled.erase( it );
			switch ( action ) {
				case rdoPlugin::maCreate: model->m_GUI_ACTION_NEW   = true; break;
				case rdoPlugin::maOpen  : model->m_GUI_ACTION_OPEN  = true; break;
				case rdoPlugin::maSave  : model->m_GUI_ACTION_SAVE  = true; break;
				case rdoPlugin::maClose : model->m_GUI_ACTION_CLOSE = true; break;
				case rdoPlugin::maBuild : model->m_GUI_ACTION_BUILD = true; break;
				case rdoPlugin::maRun   : model->m_GUI_ACTION_RUN   = true; break;
			}
		}
	}
}

void RDOStudioPlugins::actionDisable( rdoPlugin::ModelActionType action )
{
	if ( plugins && std::find( plugins->actionDisabled.begin(), plugins->actionDisabled.end(), action ) == plugins->actionDisabled.end() ) {
		plugins->actionDisabled.push_back( action );
		switch ( action ) {
			case rdoPlugin::maCreate: model->m_GUI_ACTION_NEW   = false; break;
			case rdoPlugin::maOpen  : model->m_GUI_ACTION_OPEN  = false; break;
			case rdoPlugin::maSave  : model->m_GUI_ACTION_SAVE  = false; break;
			case rdoPlugin::maClose : model->m_GUI_ACTION_CLOSE = false; break;
			case rdoPlugin::maBuild : model->m_GUI_ACTION_BUILD = false; break;
			case rdoPlugin::maRun   : model->m_GUI_ACTION_RUN   = false; break;
		}
	}
}

bool RDOStudioPlugins::actionState( rdoPlugin::ModelActionType action )
{
	if ( plugins ) {
		return std::find( plugins->actionDisabled.begin(), plugins->actionDisabled.end(), action ) == plugins->actionDisabled.end();
	}
	return false;
}

bool RDOStudioPlugins::canAction( rdoPlugin::ModelActionType action )
{
	if ( plugins ) {
		return std::find( actionDisabled.begin(), actionDisabled.end(), action ) == actionDisabled.end();
	}
	return false;
}

bool RDOStudioPlugins::newModel( const char* modelName, const char* modelPath )
{
	return model->newModel( modelName, modelPath );
}

bool RDOStudioPlugins::openModel( const char* modelName )
{
	return model->openModel( modelName ? modelName : "" );
}

bool RDOStudioPlugins::saveModel()
{
	return model->saveModel();
}

bool RDOStudioPlugins::closeModel()
{
	return model->closeModel();
}

bool RDOStudioPlugins::hasModel()
{
	return model->hasModel();
}

bool RDOStudioPlugins::isModelModify()
{
	return model->isModify();
}

bool RDOStudioPlugins::buildModel()
{
	return model->buildModel();
}

bool RDOStudioPlugins::runModel()
{
	return model->runModel();
}

bool RDOStudioPlugins::stopModel()
{
	return model->stopModel();
}

bool RDOStudioPlugins::isModelRunning()
{
	return model->isRunning();
}

rdoPlugin::ModelRuntimeMode RDOStudioPlugins::getModelRuntimeMode()
{
	switch ( model->getRuntimeMode() ) {
		case rdoRuntime::RTM_MaxSpeed  : return rdoPlugin::MRTM_MaxSpeed;
		case rdoRuntime::RTM_Jump      : return rdoPlugin::MRTM_Jump;
		case rdoRuntime::RTM_Sync      : return rdoPlugin::MRTM_Sync;
		case rdoRuntime::RTM_Pause     : return rdoPlugin::MRTM_Pause;
		case rdoRuntime::RTM_BreakPoint: return rdoPlugin::MRTM_BreakPoint;
	}
	return rdoPlugin::MRTM_MaxSpeed;
}

void RDOStudioPlugins::setModelRuntimeMode( rdoPlugin::ModelRuntimeMode runtimeMode )
{
	switch ( runtimeMode ) {
		case rdoPlugin::MRTM_MaxSpeed  : model->setRuntimeMode( rdoRuntime::RTM_MaxSpeed ); break;
		case rdoPlugin::MRTM_Jump      : model->setRuntimeMode( rdoRuntime::RTM_Jump ); break;
		case rdoPlugin::MRTM_Sync      : model->setRuntimeMode( rdoRuntime::RTM_Sync ); break;
		case rdoPlugin::MRTM_Pause     : model->setRuntimeMode( rdoRuntime::RTM_Pause ); break;
		case rdoPlugin::MRTM_BreakPoint: model->setRuntimeMode( rdoRuntime::RTM_BreakPoint ); break;
	}
}

const char* RDOStudioPlugins::getModelStructure()
{
	if ( model->hasModel() ) {
		rdo::textstream model_structure;
		model->sendMessage( kernel->simulator(), RDOThread::RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure );
		plugins->modelStructure = model_structure.str();
	} else {
		plugins->modelStructure = "";
	}
	return plugins->modelStructure.c_str();
}

bool RDOStudioPlugins::readFile( rdoPlugin::ModelFileType file_type, char** data )
{
	if ( model->hasModel() ) {
		rdoEditor::RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			rdoModelObjects::RDOFileType edit_type;
			switch ( file_type ) {
				case rdoPlugin::RTP: edit_type = rdoModelObjects::RTP; break;
				case rdoPlugin::RSS: edit_type = rdoModelObjects::RSS; break;
				case rdoPlugin::EVN: edit_type = rdoModelObjects::EVN; break;
				case rdoPlugin::PAT: edit_type = rdoModelObjects::PAT; break;
				case rdoPlugin::DPT: edit_type = rdoModelObjects::DPT; break;
				case rdoPlugin::PRC: edit_type = rdoModelObjects::PRC; break;
				case rdoPlugin::FRM: edit_type = rdoModelObjects::FRM; break;
				case rdoPlugin::FUN: edit_type = rdoModelObjects::FUN; break;
				case rdoPlugin::SMR: edit_type = rdoModelObjects::SMR; break;
				case rdoPlugin::PMD: edit_type = rdoModelObjects::PMD; break;
				case rdoPlugin::PMV: edit_type = rdoModelObjects::PMV; break;
				case rdoPlugin::TRC: edit_type = rdoModelObjects::TRC; break;
				default            : edit_type = rdoModelObjects::TRC; NEVER_REACH_HERE;
			}
			if ( tab->typeSupported( edit_type ) ) {
				rdoEditor::RDOEditorEdit* edit = tab->getItemEdit( edit_type );
				rdo::binarystream stream;
				edit->save( stream );
				tstring::size_type size = stream.str().size();
				*data = new char[size + 1];
				(*data)[size] = '\0';
				stream.str( *data );//.copy( *data, size );
				return true;
			}
		}
	}
	return false;
}

bool RDOStudioPlugins::writeFile( rdoPlugin::ModelFileType file_type, const char* data )
{
	if ( model->hasModel() ) {
		rdoEditor::RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			rdoModelObjects::RDOFileType edit_type;
			switch ( file_type ) {
				case rdoPlugin::RTP: edit_type = rdoModelObjects::RTP; break;
				case rdoPlugin::RSS: edit_type = rdoModelObjects::RSS; break;
				case rdoPlugin::EVN: edit_type = rdoModelObjects::EVN; break;
				case rdoPlugin::PAT: edit_type = rdoModelObjects::PAT; break;
				case rdoPlugin::DPT: edit_type = rdoModelObjects::DPT; break;
				case rdoPlugin::PRC: edit_type = rdoModelObjects::PRC; break;
				case rdoPlugin::FRM: edit_type = rdoModelObjects::FRM; break;
				case rdoPlugin::FUN: edit_type = rdoModelObjects::FUN; break;
				case rdoPlugin::SMR: edit_type = rdoModelObjects::SMR; break;
				case rdoPlugin::PMD: edit_type = rdoModelObjects::PMD; break;
				case rdoPlugin::PMV: edit_type = rdoModelObjects::PMV; break;
				case rdoPlugin::TRC: edit_type = rdoModelObjects::TRC; break;
				default            : edit_type = rdoModelObjects::TRC; NEVER_REACH_HERE;
			}
			if ( tab->typeSupported( edit_type ) ) {
				rdoEditor::RDOEditorEdit* edit = tab->getItemEdit( edit_type );
				rdo::binarystream stream;
				stream.str( data );
//				size_t size = strlen(data);
//				stream.resize( size );
//				memcpy( stream.data(), data, strlen(data) );
				bool readonly = edit->isReadOnly();
				if ( readonly ) edit->setReadOnly( false );
				edit->clearAll();
				edit->load( stream );
				edit->setCurrentPos( 0 );
				if ( readonly ) edit->setReadOnly( true );
				edit->updateEditGUI();
				return true;
			}
		}
	}
	return false;
}

bool RDOStudioPlugins::isFrameDescribed()
{
	return model->isFrmDescribed();
}

double RDOStudioPlugins::getFrameShowRate()
{
	return model->getShowRate();
}

void RDOStudioPlugins::setFrameShowRate( double value )
{
	model->setShowRate( value );
}

void RDOStudioPlugins::showNextFrame()
{
	model->showNextFrame();
}

void RDOStudioPlugins::showPrevFrame()
{
	model->showPrevFrame();
}

bool RDOStudioPlugins::canShowNextFrame()
{
	return model->canShowNextFrame();
}

bool RDOStudioPlugins::canShowPrevFrame()
{
	return model->canShowPrevFrame();
}

int RDOStudioPlugins::getFrameCount()
{
	return model->getFrameCount();
}

const char* RDOStudioPlugins::getFrameName( int index )
{
	return model->getFrameName( index );
}

void RDOStudioPlugins::showFrame( int index )
{
	model->showFrame( index );
}

void RDOStudioPlugins::closeAllFrame()
{
	model->closeAllFrame();
}

void RDOStudioPlugins::modelStart()
{
	if ( plugins ) {
		mutex.Lock();
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psActive );
			}
			it++;
		}
		mutex.Unlock();
	}
}

void RDOStudioPlugins::modelStop( bool model_no_error )
{
	if ( plugins ) {
		if ( model_no_error ) {
			plugins->mutex.Lock();
			rdo::textstream model_results;
			model->sendMessage( kernel->simulator(), RDOThread::RT_SIMULATOR_GET_MODEL_RESULTS, &model_results );
			tstring str = model_results.str();
			std::vector< RDOStudioPlugin* >::const_iterator it = plugins->results.begin();
			while ( it != plugins->results.end() ) {
				RDOStudioPlugin* plugin = *it;
				plugin->mutex.Lock();
				if ( !plugin->closed ) {
					plugin->results( str.c_str() );
				}
				plugin->mutex.Unlock();
				it++;
			}
			plugins->mutex.Unlock();
		}
		mutex.Lock();
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psStoped );
			}
			it++;
		}
		mutex.Unlock();
	}
}

int RDOStudioPlugins::comparePluginsByName( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		return strcmp( plugin1->getName().c_str(), plugin2->getName().c_str() );
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByVersion( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getVersionMajor() > plugin2->getVersionMajor() ) {
			return 1;;
		} else if ( plugin1->getVersionMajor() < plugin2->getVersionMajor() ) {
			return -1;
		} else if ( plugin1->getVersionMinor() > plugin2->getVersionMinor() ) {
			return 1;
		} else if ( plugin1->getVersionMinor() < plugin2->getVersionMinor() ) {
			return -1;
		} else if ( plugin1->getVersionBuild() > plugin2->getVersionBuild() ) {
			return 1;
		} else if ( plugin1->getVersionBuild() < plugin2->getVersionBuild() ) {
			return -1;
		} else {
			return strcmp( plugin1->getVersionInfo().c_str(), plugin2->getVersionInfo().c_str() );
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByRunMode( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getRunMode() > plugin2->getRunMode() ) {
			return 1;;
		} else if ( plugin1->getRunMode() < plugin2->getRunMode() ) {
			return -1;
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByState( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getState() > plugin2->getState() ) {
			return 1;;
		} else if ( plugin1->getState() < plugin2->getState() ) {
			return -1;
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByDescription( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		return strcmp( plugin1->getDescription().c_str(), plugin2->getDescription().c_str() );
	}
	return 0;
}
