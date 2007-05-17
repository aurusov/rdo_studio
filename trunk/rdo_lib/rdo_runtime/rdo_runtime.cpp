#include "pch.h"
#include "rdo_runtime.h"
#include "rdoprocess.h"
#include "rdopokaz.h"
#include "rdopokaz.h"
#include "rdodptrtime.h"
#include "rdocalc.h"
#include <limits>

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
RDOResource::RDOResource( RDORuntime* rt ):
	RDOResourceTrace( rt ),
	number( 0 ),
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
			str << (*it);
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
	result( NULL ),
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
	return fabs( terminateIfCalc->calcValueBase( this ) ) > DBL_EPSILON;
}

bool RDORuntime::setTerminateIf(RDOCalc *_terminateIfCalc)
{
	if(terminateIfCalc)
		return false;

	terminateIfCalc = _terminateIfCalc;
	return true;
}

void RDORuntime::setConstValue(int numberOfConst, RDOValue value)
{
	if(allConstants.size() <= numberOfConst)
		allConstants.resize(numberOfConst + 1);

	allConstants.at(numberOfConst) = value;
}

RDOValue RDORuntime::getConstValue(int numberOfConst)
{
	return allConstants.at( numberOfConst );
}

void RDORuntime::onEraseRes( const int res_id, const RDOCalcEraseRes* calc )
{
	RDOResource* res = allResourcesByID.at( res_id );
	if ( !res ) {
		error( rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ему ресурса: %s", calc ? calc->getName().c_str() : "неизвестное имя").c_str(), calc );
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

RDOResource* RDORuntime::createNewResource()
{
	RDOResource* res = new RDOResource( this );

	std::vector< RDOResource* >::iterator it = std::find( allResourcesByID.begin(), allResourcesByID.end(), static_cast<RDOResource*>(NULL) );
	// Нашли дырку в последовательности ресурсов
	if ( it != allResourcesByID.end() ) {
		res->number = (it - allResourcesByID.begin());
		(*it) = res;
	} else {
		res->number = allResourcesByID.size();
		allResourcesByID.push_back( res );
	}
	allResourcesByTime.push_back( res );
	return res;
}

RDOResource* RDORuntime::createNewResource( int number, bool isPermanent )
{
	allResourcesByID.resize( number + 1, NULL );
	if ( allResourcesByID.at(number) != NULL ) {
		throw RDOInternalException( "internal error N 0010" );
	}
	RDOResource* res = new RDOResource( this );
	res->number = number;
	allResourcesByID.at(number) = res;
	allResourcesByTime.push_back( res );
	if( !isPermanent ) {
		res->makeTemporary( true );
	}
	return res;
}

void RDORuntime::insertNewResource( RDOResource* res )
{
	allResourcesByID.push_back( res );
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
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it != VK_SHIFT && *it != VK_CONTROL ) {
				it = config.keyDown.erase( it );
			} else {
				it++;
			}
		}
	}
	// Подсчитаем сколько раз клавиша уже в буфере
	int cnt = 0;
	std::list< unsigned int >::iterator it = config.keyDown.begin();
	while ( it != config.keyDown.end() ) {
		if ( *it == scan_code ) {
			cnt++;
		}
		it++;
	}
	// Добавим клавишу в буфер
	if ( cnt < 4 ) {
		config.keyDown.push_back( scan_code );
	}
	if ( cnt == 0 ) key_found = true;
	return cnt > 0;
}

void RDORuntime::keyUp( unsigned int scan_code )
{
	// Если отжаты VK_SHIFT или VK_CONTROL, то сбросим удалим их из буфера
//	if ( scan_code == VK_SHIFT || scan_code == VK_CONTROL ) {
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it == scan_code ) {
				it = config.keyDown.erase( it );
			} else {
				it++;
			}
		}
//	}
}

bool RDORuntime::checkKeyPressed( unsigned int scan_code, bool shift, bool control )
{
	bool shift_found   = false;
	bool control_found = false;
	// Найдем VK_SHIFT и/или VK_CONTROL в буфере
	std::list< unsigned int >::iterator it = config.keyDown.begin();
	while ( it != config.keyDown.end() ) {
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
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it == scan_code ) {
				config.keyDown.erase( it );
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
	std::vector<std::string>::iterator it = std::find( config.activeAreasMouseClicked.begin(), config.activeAreasMouseClicked.end(), oprName );
	if ( it == config.activeAreasMouseClicked.end() ) {
		return false;
	}
	config.activeAreasMouseClicked.erase( it );
	return true;
}

bool RDORuntime::isKeyDown()
{
	return key_found || !config.activeAreasMouseClicked.empty();
}

void RDORuntime::rdoInit( RDOTrace* customTracer, RDOResult* customResult )
{
	tracer = customTracer;
	result = customResult;
	currFuncTop = 0;
	RDOSimulatorTrace::rdoInit();
}

void RDORuntime::onInit()
{
	std::for_each( initCalcs.begin(), initCalcs.end(), std::bind2nd(std::mem_fun1(&RDOCalc::calcValueBase), this) );
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

	if ( result ) {
		delete result;
		result = NULL;
	}
}

RDOValue RDORuntime::getFuncArgument(int numberOfParam)
{
	return funcStack.at(currFuncTop + numberOfParam);
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
	std::for_each( allPokaz.rbegin(), allPokaz.rend(), std::mem_fun(&RDOPokazTrace::tracePokaz) );
}

std::string RDORuntime::writePokazStructure()
{
	std::stringstream stream;
	for(int i = 0; i < allPokaz.size(); i++)
	{
		rdoRuntime::RDOPMDPokaz *curr = allPokaz.at(i);
		if(curr->trace)
			curr->writePokazStructure(stream);
	}

	return stream.str();
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
	stream << std::endl;

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

void RDORuntime::error( const char* message, const RDOCalc* calc )
{
	if ( message ) {
		errors.push_back( rdoSimulator::RDOSyntaxError( rdoSimulator::RDOSyntaxError::UNKNOWN, message, calc->error().last_line, calc->error().last_column, calc->getFileType() ) );
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
	getTracer()->stopWriting();
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
	} catch ( RDORuntimeException& e ) {
		writeExitCode();
		throw e;
	}
}

RDORuntime::RDOHotKeyToolkit::RDOHotKeyToolkit()
{														   
/*
const int RDOESCAPE	= 0x101;
const int RDOTAB		= 0x10F;
const int RDOSHIFT	= 0x12A;		// Left
const int RDOCONTROL	= 0x11D;		// Left or Right
const int RDOBACK		= 0x10E;
const int RDORETURN	= 0x11C;
const int RDOINSERT	= 0x152;
const int RDOHOME		= 0x024;
const int RDOPRIOR	= 0x149;
const int RDODELETE	= 0x153;
const int RDOEND		= 0x14F;
const int RDONEXT		= 0x151;
const int RDOUP		= 0x026;
const int RDOLEFT		= 0x025;
const int RDODOWN		= 0x028;
const int RDORIGHT	= 0x027;
const int RDODIVIDE	= 0x135;
const int RDOMULTIPLY= 0x137;		// definetly Print Screen, if somebody know 'Gray*' scan code, tell me
const int RDOSUBTRACT= 0x06D;
const int RDOADD		= 0x06B;
const int RDOCLEAR	= 0x14C;		// I think so
const int RDOSPACE	= VK_SPACE;
const int RDODECIMAL	= 0x134;		// I hope...
const int RDOF2		= 0x13C;
const int RDOF3		= 0x13D;
const int RDOF4		= 0x13E;
const int RDOF5		= 0x13F;
const int RDOF6		= 0x140;
const int RDOF7		= 0x141;
const int RDOF8		= 0x142;
const int RDOF9		= 0x143;
const int RDOF10		= 0x144;
const int RDOF11		= 0x157;
const int RDOF12		= 0x158;
const int RDONUMPAD0	= 0x152;
const int RDONUMPAD1	= 0x14F;
const int RDONUMPAD2	= 0x150;
const int RDONUMPAD3	= 0x151;
const int RDONUMPAD4	= 0x14B;
const int RDONUMPAD5	= 0x14C;
const int RDONUMPAD6	= 0x14D;
const int RDONUMPAD7	= 0x147;
const int RDONUMPAD8	= 0x148;
const int RDONUMPAD9	= 0x149;
*/
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
	keys.insert(std::map<std::string, int>::value_type("NOKEY", -1));

	for(char i = '0'; i <= '9'; i++)
		keys.insert(std::map<std::string, int>::value_type(std::string(1, i), (int)i));

	for(i = 'A'; i <= 'Z'; i++)
		keys.insert(std::map<std::string, int>::value_type(std::string(1, i), (int)i));
}

int RDORuntime::RDOHotKeyToolkit::codeFromString( std::string* key )
{
	std::map< std::string, int >::iterator it = keys.find( *key );
	if ( it == keys.end() ) {
		throw RDORuntimeException( "Unknown key name: " + *key );
	}
	return (*it).second;
}

} // namespace rdoRuntime
