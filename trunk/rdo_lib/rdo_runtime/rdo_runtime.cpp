#include "pch.h"
#include "rdo_runtime.h"
#include "rdoprocess.h"
#include "rdopokaz.h"
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

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
RDOResource::RDOResource( RDORuntime* rt, int _number, bool _trace ):
	RDOResourceTrace( rt, _number, _trace ),
//	number( _number ),
	type( 0 ),
	referenceCount( 0 )
{
}

RDOResource::~RDOResource()
{
}

bool RDOResource::operator != (RDOResource &other)
{
	if ( type != other.type ) return true;
	if ( params.size() != other.params.size() ) return true;

	int size = params.size();
	for ( int i = 0; i < size; i++ ) {
		if ( params.at(i) != other.params.at(i) ) return true;
	}
	return false;
}

std::string RDOResource::getTypeId()
{
	std::ostringstream str;
	str << type;
	return str.str();
}

std::string RDOResource::traceParametersValue()
{
	std::ostringstream str;
	if(params.size() > 0)
	{
		std::vector<RDOValue>::iterator end = params.end();
		for(std::vector<RDOValue>::iterator it = params.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			std::ostringstream _str;
			_str << *it;
			std::string::size_type pos = _str.str().find( "e" );
			if ( pos != std::string::npos ) {
				std::string __str = _str.str();
				__str.erase( pos + 2, 1 );
				str << __str.c_str();
			} else {
				str << _str.str().c_str();
			}
#else
			str << *it;
#endif
			if(++it == end)
				break;
			str << " ";
		}
	}
	return str.str();
}

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
RDORuntime::RDORuntime():
	RDOSimulatorTrace( NULL ),
	tracer( NULL ),
	results( NULL ),
	results_info( NULL ),
	lastActiveBreakPoint( NULL ),
	whyStop( rdoSimulator::EC_OK ),
	key_found( false )
{
//	runtime = this; //qq
	parent  = NULL;
	detach();
	terminateIfCalc = NULL;
}

RDORuntime::~RDORuntime()
{
	rdoDestroy();
	DeleteAllObjects( allPokaz );
}

bool RDORuntime::endCondition()
{
	if ( !terminateIfCalc ) {
		return false;	// forever
	}
	return fabs( terminateIfCalc->calcValueBase( this ).getDouble() ) > DBL_EPSILON;
}

void RDORuntime::setTerminateIf( RDOCalc* _terminateIfCalc )
{
	terminateIfCalc = _terminateIfCalc;
}

bool RDORuntime::breakPoints()
{
	std::list< BreakPoint* >::const_iterator it = breakPointsCalcs.begin();
	while ( it != breakPointsCalcs.end() ) {
		if ( (*it)->calc->calcValueBase( this ).getBool() ) {
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

void RDORuntime::setConstValue( int numberOfConst, RDOValue value )
{
	if(allConstants.size() <= numberOfConst)
		allConstants.resize(numberOfConst + 1);

	allConstants.at(numberOfConst) = value;
}

RDOValue RDORuntime::getConstValue( int numberOfConst )
{
	return allConstants.at( numberOfConst );
}

void RDORuntime::onEraseRes( const int res_id, const RDOCalcEraseRes* calc )
{
	RDOResource* res = allResourcesByID.at( res_id );
	if ( !res ) {
		error( rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ресурса: %s", calc ? calc->getName().c_str() : "неизвестное имя"), calc );
	}
	if ( res->referenceCount > 0 ) {
		error( "Невозможно удалить ресурс, т.к. он еще используется", calc );
//		error( "Try to erase used resource", fromCalc );
	} else {
		std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(rdoRuntime::RDOPMDPokaz::checkResourceErased), res) );
		allResourcesByID.at( res_id ) = NULL;
		// Диструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		allResourcesByTime.remove( res );
		delete res;
	}
}

// Вызывается только для ресурсов из RSS, во время прогона вызыват нельзя из-за allResourcesByTime
// (его надо раскомментировать, но тогда он не будет работать для RSS)
RDOResource* RDORuntime::createNewResource( int number, bool isPermanent, bool trace )
{
	if ( allResourcesByID.size() <= number + 1 ) {
		allResourcesByID.resize( number + 1, NULL );
	}
	if ( allResourcesByID.at( number ) != NULL ) {
		throw RDOInternalException( "internal error N 0010" );
	}
	RDOResource* res = new RDOResource( this, number, trace );
	allResourcesByID.at( number ) = res;
//	allResourcesByTime.push_back( res );
	allResourcesBeforeSim.push_back( res );
	if( !isPermanent ) {
		res->makeTemporary( true );
	}
	return res;
}

// Для новых ресурсов, создаваемых в процессе моделирования
RDOResource* RDORuntime::createNewResource( bool trace )
{
	RDOResource* res = new RDOResource( this, -1, trace );
	insertNewResource( res );
/*
	std::vector< RDOResource* >::iterator it = std::find( allResourcesByID.begin(), allResourcesByID.end(), static_cast<RDOResource*>(NULL) );
	// Нашли дырку в последовательности ресурсов
	if ( it != allResourcesByID.end() ) {
//		res->number = (it - allResourcesByID.begin());
//		res->id = res->number + 1;
//		res->number = res->id - 1;

//		После удаления 10-го ресурса и создании нового с номером 10 итератор возвращает указатель на 5-ый,
//		т.е. рассинхронизировались allResourcesByID и getFreeResourceId()

		int number = (it - allResourcesByID.begin());
		(*it) = res;
	} else {
//		res->number = allResourcesByID.size();
//		res->id = res->number + 1;
//		res->number = res->id - 1;
		allResourcesByID.push_back( res );
	}
*/
	return res;
}

void RDORuntime::insertNewResource( RDOResource* res )
{
	if ( res->id >= allResourcesByID.size() ) {
		allResourcesByID.resize( res->id + 1, NULL );
		allResourcesByID.at( res->id ) = res;
	} else {
		if ( allResourcesByID.at( res->id ) == NULL ) {
			allResourcesByID.at( res->id ) = res;
		} else {
			error( "Внутренняя ошибка: insertNewResource" );
		}
	}
	allResourcesByTime.push_back( res );
}

void RDORuntime::addRuntimeOperation( RDOActivityOperationRuntime* opr )
{
	addTemplateBaseOperation( opr );
}

void RDORuntime::addRuntimeRule( RDOActivityRuleRuntime* rule )
{
	addTemplateBaseOperation( rule );
}

void RDORuntime::addRuntimeIE( RDOActivityIERuntime* ie )
{
	addTemplateBaseOperation( ie );
}

void RDORuntime::addRuntimeProcess( RDOPROCProcess* process )
{
	addTemplateBaseOperation( process );
}

void RDORuntime::addRuntimeDPT( RDOSearchRuntime* dpt )
{
	addTemplateBaseOperation( dpt );
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

void RDORuntime::rdoInit( RDOTrace* customTracer, RDOResults* customResults, RDOResults* customResultsInfo )
{
	tracer       = customTracer;
	results      = customResults;
	results_info = customResultsInfo;
	currFuncTop  = 0;
	RDOSimulatorTrace::rdoInit();
}

void RDORuntime::onInit()
{
	std::for_each( initCalcs.begin(), initCalcs.end(), std::bind2nd(std::mem_fun1(&RDOCalc::calcValueBase), this) );
	std::vector< RDOResource* >::const_iterator it = allResourcesByID.begin();
	while ( it != allResourcesByID.end() ) {
		allResourcesByTime.push_back( *it );
		it++;
	}
//	std::copy( allResourcesByID.begin(), allResourcesByID.end(), allResourcesByTime.begin() );
}

RDOTrace *RDORuntime::getTracer()
{
	return tracer;
}

void RDORuntime::onDestroy()
{                    
	DeleteAllObjects( allResourcesByID );

	if ( tracer ) {
		delete tracer;
		tracer = NULL;
	}
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
	other->sizeof_sim = sizeof( RDORuntime );
	int size = allResourcesByID.size();
	for ( int i = 0; i < size; i++ ) {
		if ( allResourcesByID.at(i) == NULL ) {
			other->allResourcesByID.push_back( NULL );
		} else {
			RDOResource* res = new RDOResource( *allResourcesByID.at(i) );
			other->sizeof_sim += sizeof( RDOResource ) + sizeof( void* ) * 2;
			other->allResourcesByID.push_back( res );
			other->allResourcesByTime.push_back( res );
		}
	}
	other->allConstants = allConstants;

	return other;
}

bool RDORuntime::operator == ( RDOSimulator& other )
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

std::string RDORuntime::writeActivitiesStructure( int& counter )
{
	std::stringstream stream;
	std::vector< RDOBaseOperation* >::const_iterator it = haveBaseOperations.begin();
	while ( it != haveBaseOperations.end() ) {
		RDOActivityRuleRuntime* rule = dynamic_cast<RDOActivityRuleRuntime*>(*it);
		if ( rule ) {
			stream << counter++ << " ";
			rule->writeModelStructure( stream );
		} else {
			RDOActivityOperationRuntime* opr = dynamic_cast<RDOActivityOperationRuntime*>(*it);
			if ( opr ) {
				stream << counter++ << " ";
				opr->writeModelStructure( stream );
			}
		}
		it++;
	}
#ifdef RDOSIM_COMPATIBLE
#else
	stream << std::endl;
#endif

	int _counter = 1;
	it = haveBaseOperations.begin();
	while ( it != haveBaseOperations.end() ) {
		RDOActivityIERuntime* ie = dynamic_cast<RDOActivityIERuntime*>(*it);
		if ( ie ) {
			stream << _counter++ << " ";
			counter++;
			ie->writeModelStructure( stream );
		}
		it++;
	}

	return stream.str();
}

void RDORuntime::error( const std::string& message, const RDOCalc* calc )
{
	if ( !message.empty() ) {
		errors.push_back( rdoSimulator::RDOSyntaxError( rdoSimulator::RDOSyntaxError::UNKNOWN, rdo::format("Модельное время: %f. %s", getTimeNow(), message.c_str()), calc->src_pos().last_line, calc->src_pos().last_pos, calc->src_filetype() ) );
	}
	throw RDORuntimeException( "" );
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for(int i = 0; i < allResourcesByID.size(); i++)
	{
		if(allResourcesByID.at(i))
			allResourcesByID.at(i)->makeTemporary(false);
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
		RDOSimulatorTrace::postProcess();
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
	keys.insert(std::map<std::string, int>::value_type("ESCAPE", VK_ESCAPE));
	keys.insert(std::map<std::string, int>::value_type("TAB", VK_TAB));
	keys.insert(std::map<std::string, int>::value_type("SHIFT", VK_SHIFT));
	keys.insert(std::map<std::string, int>::value_type("CONTROL", VK_CONTROL));
	keys.insert(std::map<std::string, int>::value_type("BACK", VK_BACK));
	keys.insert(std::map<std::string, int>::value_type("RETURN", VK_RETURN));
	keys.insert(std::map<std::string, int>::value_type("INSERT", VK_INSERT));
	keys.insert(std::map<std::string, int>::value_type("HOME", VK_HOME));
	keys.insert(std::map<std::string, int>::value_type("PRIOR", VK_PRIOR));
	keys.insert(std::map<std::string, int>::value_type("DELETE", VK_DELETE));
	keys.insert(std::map<std::string, int>::value_type("END", VK_END));
	keys.insert(std::map<std::string, int>::value_type("NEXT", VK_NEXT));
	keys.insert(std::map<std::string, int>::value_type("UP", VK_UP));
	keys.insert(std::map<std::string, int>::value_type("LEFT", VK_LEFT));
	keys.insert(std::map<std::string, int>::value_type("DOWN", VK_DOWN));
	keys.insert(std::map<std::string, int>::value_type("RIGHT", VK_RIGHT));
	keys.insert(std::map<std::string, int>::value_type("DIVIDE", VK_DIVIDE));
	keys.insert(std::map<std::string, int>::value_type("MULTIPLY", VK_MULTIPLY));
	keys.insert(std::map<std::string, int>::value_type("SUBTRACT", VK_SUBTRACT));
	keys.insert(std::map<std::string, int>::value_type("ADD", VK_ADD));
	keys.insert(std::map<std::string, int>::value_type("CLEAR", VK_CLEAR));
	keys.insert(std::map<std::string, int>::value_type("SPACE", VK_SPACE));
	keys.insert(std::map<std::string, int>::value_type("DECIMAL", VK_DECIMAL));
	keys.insert(std::map<std::string, int>::value_type("F2", VK_F2));
	keys.insert(std::map<std::string, int>::value_type("F3", VK_F3));
	keys.insert(std::map<std::string, int>::value_type("F4", VK_F4));
	keys.insert(std::map<std::string, int>::value_type("F5", VK_F5));
	keys.insert(std::map<std::string, int>::value_type("F6", VK_F6));
	keys.insert(std::map<std::string, int>::value_type("F7", VK_F7));
	keys.insert(std::map<std::string, int>::value_type("F8", VK_F8));
	keys.insert(std::map<std::string, int>::value_type("F9", VK_F9));
	keys.insert(std::map<std::string, int>::value_type("F10", VK_F10));
	keys.insert(std::map<std::string, int>::value_type("F11", VK_F11));
	keys.insert(std::map<std::string, int>::value_type("F12", VK_F12));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD0", VK_NUMPAD0));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD1", VK_NUMPAD1));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD2", VK_NUMPAD2));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD3", VK_NUMPAD3));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD4", VK_NUMPAD4));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD5", VK_NUMPAD5));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD6", VK_NUMPAD6));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD7", VK_NUMPAD7));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD8", VK_NUMPAD8));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD9", VK_NUMPAD9));
	keys.insert(std::map<std::string, int>::value_type("NOKEY", 0));

	for ( char i = '0'; i <= '9'; i++ ) {
		keys.insert( std::map< std::string, int >::value_type( std::string(1, i), (int)i ) );
	}
	for ( i = 'A'; i <= 'Z'; i++ ) {
		keys.insert( std::map< std::string, int >::value_type( std::string(1, i), (int)i ) );
	}
}

int RDORuntime::RDOHotKeyToolkit::codeFromString( const std::string& key )
{
	std::map< std::string, int >::iterator it = keys.find( key );
	if ( it == keys.end() ) {
		return -1;
//		throw RDORuntimeException( "Unknown key name: " + key );
	}
	return (*it).second;
}

} // namespace rdoRuntime
