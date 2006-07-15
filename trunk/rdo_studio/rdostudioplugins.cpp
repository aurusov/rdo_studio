#include "stdafx.h"
#include "rdostudioplugins.h"
#include "rdostudioapp.h"
#include "rdostudiomodel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugin
// ----------------------------------------------------------------------------
RDOStudioPlugin::RDOStudioPlugin( const std::string& _modulName ):
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

RDOStudioPlugin::~RDOStudioPlugin()
{
	restoreState = false;
	setState( rdoPlugin::psStoped );
}

bool RDOStudioPlugin::isRDOStudioPlugin( const std::string& modulName )
{
	bool res = false;
	HMODULE local_lib = ::LoadLibrary( modulName.c_str() );
	if ( local_lib ) {
		rdoPlugin::PFunGetPluginInfo getPluginInfo = reinterpret_cast<rdoPlugin::PFunGetPluginInfo>(::GetProcAddress( local_lib, "getPluginInfo" ));
		if ( getPluginInfo ) {
			res = true;
		}
		::FreeLibrary( local_lib );
	}
	return res;
}

std::string RDOStudioPlugin::getProfilePath() const
{
	return rdo::format( "plugins\\%s_%d.%d.%d", name.c_str(), version_major, version_minor, version_build );
}

void RDOStudioPlugin::setState( const rdoPlugin::PluginState value )
{
	internalMutex.Lock();
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
				if ( startPlugin && startPlugin( plugins->getStudio() ) ) {
					plugins->mutex.Lock();
					// setup messages procedure
					rdoPlugin::PFunEnumMessages enumMessages = reinterpret_cast<rdoPlugin::PFunEnumMessages>(::GetProcAddress( lib, "enumMessages" ));
					if ( enumMessages ) {
						pluginProc = reinterpret_cast<rdoPlugin::PFunPluginProc>(::GetProcAddress( lib, "pluginProc" ));
						if ( pluginProc ) {
							while ( const int message = enumMessages() ) {
								plugins->setMessageReflect( message, pluginProc );
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
						plugins->setResults( results );
					}
					plugins->mutex.Unlock();
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
				plugins->clearMessageReflect( pluginProc );
				pluginProc = NULL;
				plugins->clearTrace( this );
				trace = NULL;
				plugins->clearResults( results );
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
	internalMutex.Unlock();
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

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
RDOStudioPlugins* plugins;

RDOStudioPlugins::RDOStudioPlugins():
	modelStructure( "" )
{
	plugins = this;

	studio.stopPlugin         = RDOStudioPlugins::stopStudioPlugin;
	studio.lock               = RDOStudioPlugins::lockPlugin;
	studio.unlock             = RDOStudioPlugins::unlockPlugin;
	studio.isClosed           = RDOStudioPlugins::isPluginClosed;

	studio.model.newModel     = RDOStudioPlugins::newModel;
	studio.model.openModel    = RDOStudioPlugins::openModel;
	studio.model.saveModel    = RDOStudioPlugins::saveModel;
	studio.model.closeModel   = RDOStudioPlugins::closeModel;
	studio.model.hasModel     = RDOStudioPlugins::hasModel;
	studio.model.isModify     = RDOStudioPlugins::isModelModify;
	studio.model.build        = RDOStudioPlugins::buildModel;
	studio.model.run          = RDOStudioPlugins::runModel;
	studio.model.stop         = RDOStudioPlugins::stopModel;
	studio.model.isRunning    = RDOStudioPlugins::isModelRunning;
	studio.model.getShowMode  = RDOStudioPlugins::getModelShowMode;
	studio.model.setShowMode  = RDOStudioPlugins::setModelShowMode;
	studio.model.getStructure = RDOStudioPlugins::getModelStructure;

	studio.frame.isDescribed = RDOStudioPlugins::isFrameDescribed;
	studio.frame.getShowRate = RDOStudioPlugins::getFrameShowRate;
	studio.frame.setShowRate = RDOStudioPlugins::setFrameShowRate;
	studio.frame.showNext    = RDOStudioPlugins::showNextFrame;
	studio.frame.showPrev    = RDOStudioPlugins::showPrevFrame;
	studio.frame.canShowNext = RDOStudioPlugins::canShowNextFrame;
	studio.frame.canShowPrev = RDOStudioPlugins::canShowPrevFrame;
	studio.frame.getCount    = RDOStudioPlugins::getFrameCount;
	studio.frame.getName     = RDOStudioPlugins::getFrameName;
	studio.frame.show        = RDOStudioPlugins::showFrame;
	studio.frame.closeAll    = RDOStudioPlugins::closeAllFrame;

//	kernel.setNotifyReflect( RDOKernel::beforeModelStart, modelStartNotify );
//	kernel.setNotifyReflect( RDOKernel::endExecuteModel, endExecuteModelNotify );
//	kernel.setNotifyReflect( RDOKernel::modelStopped, modelStopNotify );
//	kernel.setNotifyReflect( RDOKernel::executeError, modelStopNotify );
//	kernel.setNotifyReflect( RDOKernel::traceString, traceNotify );

	init();
}

RDOStudioPlugins::~RDOStudioPlugins()
{
	std::vector< RDOStudioPlugin* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();

	plugins = NULL;
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
			} else if ( !finder.IsDots() ) {
				std::string fullname = finder.GetFilePath();
				if ( fullname.rfind( ".rsp" ) == fullname.length() - 4 ) {
					std::string modulName = finder.GetFilePath();
					if ( RDOStudioPlugin::isRDOStudioPlugin( modulName ) ) {
						RDOStudioPlugin* plugin = new RDOStudioPlugin( modulName );
						list.push_back( plugin );
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
		path = rdo::extractFilePath( szExeName );
	}
	if ( !path.empty() ) {
		enumPlugins( path + "\\plugins\\*.*" );
	}
}

void RDOStudioPlugins::setMessageReflect( const int message, rdoPlugin::PFunPluginProc pluginProc )
{
	mutex.Lock();
	if ( message && pluginProc ) {
		bool flag = true;
		messageList::iterator it = messages.find( message );
		while ( it != messages.end() ) {
			if ( (*it).second == pluginProc ) {
				flag = false;
				break;
			}
			it++;
		}
		if ( flag ) {
			messages.insert( messageList::value_type( message, pluginProc ) );
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearMessageReflect( rdoPlugin::PFunPluginProc pluginProc )
{
	mutex.Lock();
	messageList::iterator it = messages.begin();
	while ( it != messages.end() ) {
		if ( (*it).second == pluginProc ) {
			it = messages.erase( it );
		} else {
			it++;
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::pluginProc( const int message )
{
	mutex.Lock();
	messageList::iterator it = messages.lower_bound( message );
	while ( it != messages.upper_bound( message ) ) {
		(*it).second( message );
		it++;
	}
	mutex.Unlock();
}

void RDOStudioPlugins::setTrace( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	if ( plugin ) {
		std::vector< RDOStudioPlugin* >::const_iterator it = trace.begin();
		bool flag = true;
		while ( it != trace.end() ) {
			if ( *it == plugin ) {
				flag = false;
				break;
			}
			it++;
		}
		if ( flag ) {
			trace.push_back( plugin );
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearTrace( RDOStudioPlugin* plugin )
{
	mutex.Lock();
	std::vector< RDOStudioPlugin* >::iterator it = trace.begin();
	while ( it != trace.end() ) {
		if ( *it == plugin ) {
			trace.erase( it );
			break;
		}
		it++;
	}
	mutex.Unlock();
}

void RDOStudioPlugins::setResults( rdoPlugin::PFunResults _results )
{
	mutex.Lock();
	if ( _results ) {
		std::vector< rdoPlugin::PFunResults >::const_iterator it = results.begin();
		bool flag = true;
		while ( it != results.end() ) {
			if ( *it == _results ) {
				flag = false;
				break;
			}
			it++;
		}
		if ( flag ) {
			results.push_back( _results );
		}
	}
	mutex.Unlock();
}

void RDOStudioPlugins::clearResults( rdoPlugin::PFunResults _results )
{
	mutex.Lock();
	std::vector< rdoPlugin::PFunResults >::iterator it = results.begin();
	while ( it != results.end() ) {
		if ( *it == _results ) {
			results.erase( it );
			break;
		}
		it++;
	}
	mutex.Unlock();
}

void RDOStudioPlugins::stopStudioPlugin( const HMODULE lib )
{
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
		plugin->internalMutex.Lock();
		plugin->closed = true;
	}
	AfxGetApp()->PostThreadMessage( PLUGIN_MUSTEXIT_MESSAGE, reinterpret_cast<WPARAM>(lib), 0 );
	if ( plugin ) {
		plugin->internalMutex.Unlock();
	}
}

void RDOStudioPlugins::lockPlugin( const HMODULE lib )
{
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
	while ( it != plugins->list.end() ) {
		RDOStudioPlugin* plugin = *it;
		if ( plugin->lib == lib ) {
			plugin->externalMutex.Lock();
			break;
		}
		it++;
	}
}

void RDOStudioPlugins::unlockPlugin( const HMODULE lib )
{
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
	while ( it != plugins->list.end() ) {
		RDOStudioPlugin* plugin = *it;
		if ( plugin->lib == lib ) {
			plugin->externalMutex.Unlock();
			break;
		}
		it++;
	}
}

bool RDOStudioPlugins::isPluginClosed( const HMODULE lib )
{
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
	while ( it != plugins->list.end() ) {
		RDOStudioPlugin* plugin = *it;
		if ( plugin->lib == lib ) {
			return plugin->closed;
		}
		it++;
	}
	return false;
}

void RDOStudioPlugins::stopPlugin( const HMODULE lib )
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::const_iterator it = plugins->list.begin();
		while ( it != plugins->list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->lib == lib ) {
				plugin->setState( rdoPlugin::psStoped );
				break;
			}
			it++;
		}
	}
}

void RDOStudioPlugins::newModel()
{
	model->newModel();
}

bool RDOStudioPlugins::openModel( const char* modelName )
{
	return model->openModel( modelName ? modelName : "" );
}

void RDOStudioPlugins::saveModel()
{
	model->saveModel();
}

void RDOStudioPlugins::closeModel()
{
	model->closeModel();
}

bool RDOStudioPlugins::hasModel()
{
	return model->hasModel();
}

bool RDOStudioPlugins::isModelModify()
{
	return model->isModify();
}

void RDOStudioPlugins::buildModel()
{
	model->buildModel();
}

void RDOStudioPlugins::runModel()
{
	model->runModel();
}

void RDOStudioPlugins::stopModel()
{
	model->stopModel();
}

bool RDOStudioPlugins::isModelRunning()
{
	return model->isRunning();
}

rdoPlugin::ModelShowMode RDOStudioPlugins::getModelShowMode()
{
	switch ( model->getShowMode() ) {
		case rdosim::SM_NoShow   : return rdoPlugin::NoShow;
		case rdosim::SM_Animation: return rdoPlugin::Animation;
		case rdosim::SM_Monitor  : return rdoPlugin::Monitor;
	}
	return rdoPlugin::NoShow;
}

void RDOStudioPlugins::setModelShowMode( rdoPlugin::ModelShowMode showMode )
{
	switch ( showMode ) {
		case rdoPlugin::NoShow   : model->setShowMode( rdosim::SM_NoShow ); break;
		case rdoPlugin::Animation: model->setShowMode( rdosim::SM_Animation ); break;
		case rdoPlugin::Monitor  : model->setShowMode( rdosim::SM_Monitor ); break;
	}
}

const char* RDOStudioPlugins::getModelStructure()
{
	if ( model->hasModel() ) {
		plugins->modelStructure = kernel->simulator()->getModelStructure().str();
	} else {
		plugins->modelStructure = "";
	}
	return plugins->modelStructure.c_str();
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

int RDOStudioPlugins::modelStartNotify( void* )
{
	AfxGetApp()->PostThreadMessage( PLUGIN_STARTMODEL_MESSAGE, 0, 0 );
	return 1;
}

int RDOStudioPlugins::endExecuteModelNotify( void* )
{
	plugins->mutex.Lock();
	std::string str = kernel->simulator()->getResults().str();
	std::vector< rdoPlugin::PFunResults>::const_iterator it = plugins->results.begin();
	while ( it != plugins->results.end() ) {
		(*it++)( str.c_str() );
	}
	plugins->mutex.Unlock();
	AfxGetApp()->PostThreadMessage( PLUGIN_STOPMODEL_MESSAGE, 0, 0 );
	return 1;
}

int RDOStudioPlugins::modelStopNotify( void* )
{
	AfxGetApp()->PostThreadMessage( PLUGIN_STOPMODEL_MESSAGE, 0, 0 );
	return 1;
}

int RDOStudioPlugins::traceNotify( void* str )
{
	plugins->mutex.Lock();
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->trace.begin();
	while ( it != plugins->trace.end() ) {
		RDOStudioPlugin* plugin = *it;
		plugin->internalMutex.Lock();
		if ( !plugin->closed ) {
			plugin->trace( ((std::string*)str)->c_str() );
		}
		plugin->internalMutex.Unlock();
		it++;
	}
	plugins->mutex.Unlock();
	return 1;
}

void RDOStudioPlugins::modelStart()
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psActive );
			}
			it++;
		}
	}
}

void RDOStudioPlugins::modelStop()
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psStoped );
			}
			it++;
		}
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
