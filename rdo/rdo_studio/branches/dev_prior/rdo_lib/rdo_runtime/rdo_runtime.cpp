#include "pch.h"
#include "rdo_runtime.h"
#include "rdo_activity.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"
#include "rdoprocess.h"
#include "rdopokaz.h"
#include "rdodptrtime.h"
#include "rdocalc.h"
#include <limits>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
RDORuntime::RDORuntime():
	RDOSimulatorTrace(),
	m_currActivity( NULL ),
	results( NULL ),
	results_info( NULL ),
	lastActiveBreakPoint( NULL ),
	whyStop( rdoSimulator::EC_OK ),
	key_found( false )
{
	m_parent = NULL;
	detach();
	terminateIfCalc = NULL;
}

RDORuntime::~RDORuntime()
{
	connected.clear();
	onDestroy();
	rdo::deleteAllObjects( allPokaz );
}

void RDORuntime::connect( RDORuntimeObject* to, UINT message )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		if ( it->second == to ) break;
		it++;
	}
	if ( it == connected.end() ) {
		connected.insert( Connected::value_type( message, to ) );
	}
}

void RDORuntime::disconnect( RDORuntimeObject* to )
{
	Connected::iterator it = connected.begin();
	while ( it != connected.end() ) {
		if ( it->second == to ) {
			it = connected.erase( it );
			if ( it == connected.end() ) break;
		}
		it++;
	}
}

void RDORuntime::disconnect( RDORuntimeObject* to, UINT message )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		if ( it->second == to ) {
			it = connected.erase( it );
			if ( it == connected.end() ) break;
		}
		it++;
	}
}

void RDORuntime::fireMessage( RDORuntimeObject* from, unsigned int message, void* param )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		it->second->notify( from, message, param );
		it++;
	}
}

bool RDORuntime::endCondition()
{
	if ( !terminateIfCalc ) {
		return false;	// forever
	}
	return fabs( terminateIfCalc->calcValue( this ).getDouble() ) > DBL_EPSILON;
}

void RDORuntime::setTerminateIf( RDOCalc* _terminateIfCalc )
{
	terminateIfCalc = _terminateIfCalc;
}

bool RDORuntime::breakPoints()
{
	std::list< BreakPoint* >::const_iterator it = breakPointsCalcs.begin();
	while ( it != breakPointsCalcs.end() ) {
		if ( (*it)->calc->calcValue( this ).getAsBool() ) {
			lastActiveBreakPoint = *it;
			return true;
		}
		it++;
	}
	return false;
}

void RDORuntime::insertBreakPoint( const std::string& name, RDOCalc* calc )
{
	breakPointsCalcs.push_back( new BreakPoint( this, name, calc ) );
}

RDOCalc* RDORuntime::findBreakPoint( const std::string& name )
{
	std::list< BreakPoint* >::const_iterator it = breakPointsCalcs.begin();
	while ( it != breakPointsCalcs.end() ) {
		if ( (*it)->name == name ) {
			return (*it)->calc;
		}
		it++;
	}
	return NULL;
}

std::string RDORuntime::getLastBreakPointName() const
{
	return lastActiveBreakPoint ? lastActiveBreakPoint->name + ": " + lastActiveBreakPoint->calc->src_text() : "";
}

void RDORuntime::setConstValue( unsigned int numberOfConst, RDOValue value )
{
	if ( allConstants.size() <= numberOfConst )
	{
		allConstants.resize(numberOfConst + 1);
	}
	allConstants.at(numberOfConst) = value;
}

RDOValue RDORuntime::getConstValue( int numberOfConst )
{
	return allConstants.at( numberOfConst );
}

#ifdef _DEBUG
bool RDORuntime::checkState()
{
	if ( state.empty() ) {
		std::vector< RDOValue > res;
		res.push_back( 1 );
		res.push_back( 4 );
		state.push_back( res );
		res[0] = 2;
		res[1] = 1;
		state.push_back( res );
		res[0] = 3;
		res[1] = 3;
		state.push_back( res );
		res[0] = 4;
		res[1] = 2;
		state.push_back( res );
		res[0] = 5;
		res[1] = 6;
		state.push_back( res );
		res.clear();
		res.push_back( 5 );
		state.push_back( res );
	}
	if ( state.size() != allResourcesByID.size() ) return false;
	for ( unsigned int i = 0; i < state.size(); i++ ) {
		if ( state[i].size() != allResourcesByID[i]->paramsCount() ) return false;
		for ( unsigned int j = 0; j < allResourcesByID[i]->paramsCount(); j++ )
		{
			if ( state[i][j] != allResourcesByID[i]->getParam(j) ) return false;
		}
	}
	return true;
}

void RDORuntime::showResources( int node ) const
{
	TRACE( "------------- %d:\n", node );
	int index = 0;
	std::vector< RDOResource* >::const_iterator it = allResourcesByID.begin();
	while ( it != allResourcesByID.end() ) {
		if ( *it ) {
			TRACE( "%d. ", index );
			for ( unsigned int i = 0; i < (*it)->paramsCount(); i++ )
			{
				TRACE( "%s ", (*it)->getParam(i).getAsString().c_str() );
			}
			TRACE( "\n" );
		} else {
			TRACE( "%d. NULL\n", index );
		}
		index++;
		it++;
	}
}
#endif

void RDORuntime::onEraseRes( const int res_id, const RDOCalcEraseRes* calc )
{
	RDOResource* res = allResourcesByID.at( res_id );
	if ( !res ) {
		error( rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ресурса: %s", calc ? calc->getName().c_str() : "неизвестное имя"), calc );
	}
	if ( !res->canFree() ) {
		error( "Невозможно удалить ресурс, т.к. он еще используется", calc );
//		error( "Try to erase used resource", fromCalc );
	} else {
		std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&rdoRuntime::RDOPMDPokaz::checkResourceErased), res) );
		allResourcesByID.at( res_id ) = NULL;
		// Диструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		allResourcesByTime.remove( res );
		delete res;
	}
}

// Вызывается только для ресурсов из RSS, во время прогона вызыват нельзя из-за allResourcesByTime
// (его надо раскомментировать, но тогда он не будет работать для RSS)
RDOResource* RDORuntime::createNewResource( unsigned int type, RDOCalcCreateNumberedResource* calc )
{
	if ( allResourcesByID.size() <= calc->getNumber() + 1 ) {
		allResourcesByID.resize( calc->getNumber() + 1, NULL );
	}
	if ( allResourcesByID.at( calc->getNumber() ) != NULL ) {
		throw RDOInternalException( "internal error N 0010" );
	}
	RDOResource* res = calc->createResource( this );
	allResourcesByID.at( calc->getNumber() ) = res;
	allResourcesBeforeSim.push_back( res );
	return res;
}

// Для новых ресурсов, создаваемых в процессе моделирования
RDOResource* RDORuntime::createNewResource( unsigned int type, bool trace )
{
	RDOResource* res = new RDOResource( this, -1, type, trace );
	insertNewResource( res );
	return res;
}

void RDORuntime::insertNewResource( RDOResource* res )
{
	if ( res->getTraceID() >= allResourcesByID.size() ) {
		allResourcesByID.resize( res->getTraceID() + 1, NULL );
		allResourcesByID.at( res->getTraceID() ) = res;
	} else {
		if ( allResourcesByID.at( res->getTraceID() ) == NULL ) {
			allResourcesByID.at( res->getTraceID() ) = res;
		} else {
			error( "Внутренняя ошибка: insertNewResource" );
		}
	}
	allResourcesByTime.push_back( res );
}

void RDORuntime::addRuntimeIE( RDOIrregEvent* ie )
{
	appendBaseOperation( ie );
}

void RDORuntime::addRuntimeRule( RDORule* rule )
{
	appendBaseOperation( rule );
}

void RDORuntime::addRuntimeOperation( RDOOperation* opr )
{
	appendBaseOperation( opr );
}

void RDORuntime::addRuntimePokaz( RDOPMDPokaz* pok )
{
	allPokaz.push_back( pok );
}

void RDORuntime::addRuntimeFrame( RDOFRMFrame* frame )
{ 
	allFrames.push_back( frame ); 
}

RDOFRMFrame* RDORuntime::lastFrame() const
{
	return !allFrames.empty() ? allFrames.front() : NULL;
}

bool RDORuntime::keyDown( unsigned int scan_code )
{
	// Если нажаты VK_SHIFT или VK_CONTROL, то сбросим буфер клавиатуры
	if ( scan_code == VK_SHIFT || scan_code == VK_CONTROL ) {
		std::list< unsigned int >::iterator it = keysDown.begin();
		while ( it != keysDown.end() ) {
			if ( *it != VK_SHIFT && *it != VK_CONTROL ) {
				it = keysDown.erase( it );
			} else {
				it++;
			}
		}
	}
	// Подсчитаем сколько раз клавиша уже в буфере
	int cnt = 0;
	std::list< unsigned int >::iterator it = keysDown.begin();
	while ( it != keysDown.end() ) {
		if ( *it == scan_code ) {
			cnt++;
		}
		it++;
	}
	// Добавим клавишу в буфер
	if ( cnt < 4 ) {
		keysDown.push_back( scan_code );
	}
	if ( cnt == 0 ) key_found = true;
	return cnt > 0;
}

void RDORuntime::keyUp( unsigned int scan_code )
{
	// Если отжаты VK_SHIFT или VK_CONTROL, то сбросим удалим их из буфера
//	if ( scan_code == VK_SHIFT || scan_code == VK_CONTROL ) {
		std::list< unsigned int >::iterator it = keysDown.begin();
		while ( it != keysDown.end() ) {
			if ( *it == scan_code ) {
				it = keysDown.erase( it );
			} else {
				it++;
			}
		}
//	}
}

bool RDORuntime::checkKeyPressed( unsigned int scan_code, bool shift, bool control )
{
	if ( scan_code == 0 ) return false;
	bool shift_found   = false;
	bool control_found = false;
	// Найдем VK_SHIFT и/или VK_CONTROL в буфере
	std::list< unsigned int >::iterator it = keysDown.begin();
	while ( it != keysDown.end() ) {
		if ( *it == VK_SHIFT ) {
			shift_found = true;
			if ( shift_found && control_found ) break;
		}
		if ( *it == VK_CONTROL ) {
			control_found = true;
			if ( shift_found && control_found ) break;
		}
		it++;
	}
	// Теперь найдем саму клавишу в буфере
	// Удалим её из буфера перед выходом
	if ( shift_found == shift && control_found == control ) {
		std::list< unsigned int >::iterator it = keysDown.begin();
		while ( it != keysDown.end() ) {
			if ( *it == scan_code ) {
				keysDown.erase( it );
				key_found = true;
				return true;
			}
			it++;
		}
	}
	key_found = false;
	return false;
}

bool RDORuntime::checkAreaActivated( const std::string& oprName )
{
	std::vector<std::string>::iterator it = std::find( activeAreasMouseClicked.begin(), activeAreasMouseClicked.end(), oprName );
	if ( it == activeAreasMouseClicked.end() ) {
		return false;
	}
	activeAreasMouseClicked.erase( it );
	return true;
}

bool RDORuntime::isKeyDown()
{
	return key_found || !activeAreasMouseClicked.empty();
}

void RDORuntime::rdoInit( RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo )
{
	m_tracer     = tracer;
	results      = customResults;
	results_info = customResultsInfo;
	currFuncTop  = 0;
	Parent::rdoInit();
}

void RDORuntime::onInit()
{
	std::for_each( initCalcs.begin(), initCalcs.end(), std::bind2nd(std::mem_fun1(&RDOCalc::calcValue), this) );
	std::vector< RDOResource* >::const_iterator it = allResourcesByID.begin();
	while ( it != allResourcesByID.end() ) {
		allResourcesByTime.push_back( *it );
		it++;
	}
//	std::copy( allResourcesByID.begin(), allResourcesByID.end(), allResourcesByTime.begin() );
}

void RDORuntime::onDestroy()
{                    
	rdo::deleteAllObjects( allResourcesByID );

	if ( results ) {
		delete results;
		results = NULL;
	}
	if ( results_info ) {
		delete results_info;
		results_info = NULL;
	}
}

RDOValue RDORuntime::getFuncArgument( int numberOfParam )
{
	return funcStack.at( currFuncTop + numberOfParam );
}

RDOSimulator* RDORuntime::clone()
{
	RDORuntime* other = new RDORuntime();
	other->m_sizeof_sim = sizeof( RDORuntime );

	*other = *this;

	return other;
}

void RDORuntime::operator= (const RDORuntime& other)
{
	int size = other.allResourcesByID.size();
	for ( int i = 0; i < size; i++ )
	{
		if ( other.allResourcesByID.at(i) == NULL )
		{
			allResourcesByID.push_back(NULL);
		}
		else
		{
			RDOResource* res = new RDOResource( *other.allResourcesByID.at(i) );
			res->setRuntime(this);
			res->setTraceID( res->getTraceID(), res->getTraceID() + 1 );
			m_sizeof_sim += sizeof( RDOResource ) + sizeof( void* ) * 2;
			allResourcesByID.push_back( res );
			allResourcesByTime.push_back( res );
		}
	}
	allConstants = other.allConstants;

	Parent::operator= (*static_cast<const Parent*>(&other));
}

bool RDORuntime::operator== (RDOSimulator& other)
{
	RDORuntime* otherRuntime = dynamic_cast<RDORuntime*>(&other);

	if ( otherRuntime->allResourcesByID.size() != allResourcesByID.size() ) return false;

	int size = allResourcesByID.size();
	for ( int i = 0; i < size; i++ ) {
		if ( allResourcesByID.at(i) == NULL && otherRuntime->allResourcesByID.at(i) != NULL ) return false;
		if ( allResourcesByID.at(i) != NULL && otherRuntime->allResourcesByID.at(i) == NULL ) return false;
		if ( allResourcesByID.at(i) == NULL && otherRuntime->allResourcesByID.at(i) == NULL ) continue;
		if ( *otherRuntime->allResourcesByID.at(i) != *allResourcesByID.at(i) ) return false;
	}
	return true;
}

void RDORuntime::onResetPokaz()
{
	std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::resetPokaz), this) );
}

void RDORuntime::onCheckPokaz()
{
	std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::checkPokaz), this) );
}

void RDORuntime::onAfterCheckPokaz()
{
	std::for_each( allPokaz.begin(), allPokaz.end(), std::mem_fun(&RDOPokazTrace::tracePokaz) );
}

void RDORuntime::error( const std::string& message, const RDOCalc* calc )
{
	if ( !message.empty() ) {
		errors.push_back( rdoSimulator::RDOSyntaxError( rdoSimulator::RDOSyntaxError::UNKNOWN, rdo::format("Модельное время: %f. %s", getTimeNow(), message.c_str()), calc ? calc->src_pos().m_last_line : 0, calc ? calc->src_pos().m_last_pos : 0, calc ? calc->src_filetype() : rdoModelObjects::PAT ) );
	}
	throw RDORuntimeException( "" );
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for ( unsigned int i = 0; i < allResourcesByID.size(); i++ )
	{
		if ( allResourcesByID.at(i) )
		{
			allResourcesByID.at(i)->makeTemporary(false);
		}
	}
}

void RDORuntime::writeExitCode()
{
	switch ( whyStop ) {
		case rdoSimulator::EC_OK:
			getTracer()->writeStatus( this, "NORMAL_TERMINATION" );
			break;
		case rdoSimulator::EC_NoMoreEvents:
			getTracer()->writeStatus( this, "NO_MORE_EVENTS" );
			break;
		case rdoSimulator::EC_RunTimeError:
			getTracer()->writeStatus( this, "RUN_TIME_ERROR" );
			break;
		case rdoSimulator::EC_UserBreak:
			getTracer()->writeStatus( this, "USER_BREAK" );
			break;
	}
}

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
	try {
		std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::calcStat), this) );
	} catch ( RDORuntimeException& ) {
	}
	try {
		Parent::postProcess();
		writeExitCode();
		getTracer()->stopWriting();
	} catch ( RDORuntimeException& e ) {
		writeExitCode();
		getTracer()->stopWriting();
		throw e;
	}
}

RDORuntime::RDOHotKeyToolkit::RDOHotKeyToolkit()
{														   
	m_keys.insert(KeySet::value_type("ESCAPE", VK_ESCAPE));
	m_keys.insert(KeySet::value_type("TAB", VK_TAB));
	m_keys.insert(KeySet::value_type("SHIFT", VK_SHIFT));
	m_keys.insert(KeySet::value_type("CONTROL", VK_CONTROL));
	m_keys.insert(KeySet::value_type("BACK", VK_BACK));
	m_keys.insert(KeySet::value_type("RETURN", VK_RETURN));
	m_keys.insert(KeySet::value_type("INSERT", VK_INSERT));
	m_keys.insert(KeySet::value_type("HOME", VK_HOME));
	m_keys.insert(KeySet::value_type("PRIOR", VK_PRIOR));
	m_keys.insert(KeySet::value_type("DELETE", VK_DELETE));
	m_keys.insert(KeySet::value_type("END", VK_END));
	m_keys.insert(KeySet::value_type("NEXT", VK_NEXT));
	m_keys.insert(KeySet::value_type("UP", VK_UP));
	m_keys.insert(KeySet::value_type("LEFT", VK_LEFT));
	m_keys.insert(KeySet::value_type("DOWN", VK_DOWN));
	m_keys.insert(KeySet::value_type("RIGHT", VK_RIGHT));
	m_keys.insert(KeySet::value_type("DIVIDE", VK_DIVIDE));
	m_keys.insert(KeySet::value_type("MULTIPLY", VK_MULTIPLY));
	m_keys.insert(KeySet::value_type("SUBTRACT", VK_SUBTRACT));
	m_keys.insert(KeySet::value_type("ADD", VK_ADD));
	m_keys.insert(KeySet::value_type("CLEAR", VK_CLEAR));
	m_keys.insert(KeySet::value_type("SPACE", VK_SPACE));
	m_keys.insert(KeySet::value_type("DECIMAL", VK_DECIMAL));
	m_keys.insert(KeySet::value_type("F2", VK_F2));
	m_keys.insert(KeySet::value_type("F3", VK_F3));
	m_keys.insert(KeySet::value_type("F4", VK_F4));
	m_keys.insert(KeySet::value_type("F5", VK_F5));
	m_keys.insert(KeySet::value_type("F6", VK_F6));
	m_keys.insert(KeySet::value_type("F7", VK_F7));
	m_keys.insert(KeySet::value_type("F8", VK_F8));
	m_keys.insert(KeySet::value_type("F9", VK_F9));
	m_keys.insert(KeySet::value_type("F10", VK_F10));
	m_keys.insert(KeySet::value_type("F11", VK_F11));
	m_keys.insert(KeySet::value_type("F12", VK_F12));
	m_keys.insert(KeySet::value_type("NUMPAD0", VK_NUMPAD0));
	m_keys.insert(KeySet::value_type("NUMPAD1", VK_NUMPAD1));
	m_keys.insert(KeySet::value_type("NUMPAD2", VK_NUMPAD2));
	m_keys.insert(KeySet::value_type("NUMPAD3", VK_NUMPAD3));
	m_keys.insert(KeySet::value_type("NUMPAD4", VK_NUMPAD4));
	m_keys.insert(KeySet::value_type("NUMPAD5", VK_NUMPAD5));
	m_keys.insert(KeySet::value_type("NUMPAD6", VK_NUMPAD6));
	m_keys.insert(KeySet::value_type("NUMPAD7", VK_NUMPAD7));
	m_keys.insert(KeySet::value_type("NUMPAD8", VK_NUMPAD8));
	m_keys.insert(KeySet::value_type("NUMPAD9", VK_NUMPAD9));
	m_keys.insert(KeySet::value_type("NOKEY", 0));

	for ( char i = '0'; i <= '9'; i++ ) {
		m_keys.insert( KeySet::value_type( std::string(1, i), (KeyCode)i ) );
	}
	for ( char i = 'A'; i <= 'Z'; i++ ) {
		m_keys.insert( KeySet::value_type( std::string(1, i), (KeyCode)i ) );
	}
}

RDORuntime::RDOHotKeyToolkit::KeyCode RDORuntime::RDOHotKeyToolkit::codeFromString( const std::string& key ) const
{
	CIterator it = m_keys.find( key );
	if ( it == m_keys.end() )
	{
		return UNDEFINED_KEY;
//		throw RDORuntimeException( "Unknown key name: " + key );
	}
	return (*it).second;
}

} // namespace rdoRuntime
