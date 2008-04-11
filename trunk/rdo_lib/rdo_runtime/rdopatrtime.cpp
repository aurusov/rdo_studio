#include "pch.h"
#include "rdopatrtime.h"
#include "rdo_runtime.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
RDOPattern::RDOPattern( RDORuntime* _runtime, bool _trace ):
	RDORuntimeParent( _runtime ),
	trace( _trace )
{
}

RDOActivityRuntime* RDOPattern::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	throw RDOInternalException( "internal error 0009" );
}

bool RDOPattern::choiceFrom( RDOSimulator* sim )
{
	RDORuntime* runtime = (RDORuntime*)sim;
	int size = choiceFromCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		if ( !choiceFromCalcs[i]->calcValueBase( runtime ).getBool() ) return false;
	}
	return true;
}

void RDOPattern::convertBegin( RDORuntime* _runtime )
{
	int size = beginCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		beginCalcs.at(i)->calcValueBase( _runtime );
	}
}

void RDOPattern::convertBeginErase( RDORuntime* _runtime )
{
	int size = beginEraseCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		beginEraseCalcs.at(i)->calcValueBase( _runtime );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPATRule
// ----------------------------------------------------------------------------
RDOPATRule::RDOPATRule( RDORuntime* rTime, bool _trace ):
	RDOPattern( rTime, _trace )
{
}

void RDOPATRule::convertRule( RDOSimulator* sim )
{
	RDOPattern::convertBegin( static_cast<RDORuntime*>(sim) );
}

RDOActivityRuntime* RDOPATRule::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityRule* rule = new RDOActivityRule( runtime, this, trace, _oprName );
	runtime->addRuntimeRule( rule );
	return rule;
}

RDOActivityRuntime* RDOPATRule::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityRule* rule = new RDOActivityRule( runtime, this, trace, condition, _oprName );
	runtime->addRuntimeRule( rule );
	return rule;
}

// ----------------------------------------------------------------------------
// ---------- RDOPATIrregEvent
// ----------------------------------------------------------------------------
RDOPATIrregEvent::RDOPATIrregEvent( RDORuntime* rTime, bool _trace ):
	RDOPattern( rTime, _trace ),
	timeCalc( NULL )
{
}

double RDOPATIrregEvent::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeCalc->calcValueBase( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), timeCalc );
	return 0;
}

void RDOPATIrregEvent::convertEvent( RDOSimulator* sim )
{
	choiceFrom( sim ); // to set permanent resource relevent numbers
	RDOPattern::convertBegin( static_cast<RDORuntime*>(sim) );
}

RDOActivityRuntime* RDOPATIrregEvent::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityIrregEvent* ie = new RDOActivityIrregEvent( runtime, this, trace, _oprName );
	runtime->addRuntimeIE( ie );
	return ie;
}

// ----------------------------------------------------------------------------
// ---------- RDOPATOperation
// ----------------------------------------------------------------------------
RDOPATOperation::RDOPATOperation( RDORuntime* rTime, bool _trace ):
	RDOPattern( rTime, _trace ),
	timeCalc( NULL )
{
}

double RDOPATOperation::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeCalc->calcValueBase( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать окончание операции в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), timeCalc );
	return 0;
}

void RDOPATOperation::convertBegin( RDOSimulator* sim )
{
	RDOPattern::convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOPATOperation::convertEnd( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = endCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		endCalcs.at(i)->calcValueBase( runtime );
	}
}

void RDOPATOperation::convertEndErase( RDORuntime* _runtime )
{
	int size = endEraseCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		endEraseCalcs.at(i)->calcValueBase( _runtime );
	}
}

RDOActivityRuntime* RDOPATOperation::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityOperation* oper = new RDOActivityOperation( runtime, this, trace, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityRuntime* RDOPATOperation::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName)
{
	RDOActivityOperation* oper = new RDOActivityOperation( runtime, this, trace, condition, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

// ----------------------------------------------------------------------------
// ---------- RDOPATKeyboard
// ----------------------------------------------------------------------------
RDOPATKeyboard::RDOPATKeyboard( RDORuntime* rTime, bool _trace ):
	RDOPATOperation( rTime, _trace )
{
}

RDOActivityRuntime* RDOPATKeyboard::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityKeyboard* oper = new RDOActivityKeyboard( runtime, this, trace, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityRuntime* RDOPATKeyboard::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityKeyboard* oper = new RDOActivityKeyboard( runtime, this, trace, condition, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityRuntime
// ----------------------------------------------------------------------------
void RDOActivityRuntime::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = paramsCalcs.size();
	for(int i = 0; i < size; i++)
		paramsCalcs.at(i)->calcValueBase( runtime );
}

void RDOActivityRuntime::getRelevantResources( RDOSimulator* sim, std::list< RDOResourceTrace* >& rel_res_list )
{
	rel_res_list.clear();
	int size = relResID.size();
	for ( int i = 0; i < size; i++ ) {
		rel_res_list.push_back( static_cast<RDORuntime*>(sim)->getResourceByID( relResID[i] ) );
	}
}

void RDOActivityRuntime::updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResourceTrace::ConvertStatus >& status_list )
{
	updateRelRes( sim );
	int i = 0;
	std::list< RDOResourceTrace* >::iterator it = relevantResources.begin();
	while ( it != relevantResources.end() ) {
		RDOResourceTrace* res = *it;
		if ( res ) {
			res->setState( status_list[i] );
			switch ( status_list[i] ) {
				case RDOResourceTrace::CS_Create: {
					res->makeTemporary( true );
					break;
				}
			}
		}
		i++;
		it++;
	}
}

std::string RDOActivityRuntime::traceResourcesList( char prefix, RDOSimulatorTrace* sim )
{
	std::string res;
	for ( std::list< RDOResourceTrace* >::const_iterator i = relevantResources.begin(); i != relevantResources.end(); i++ ) {
		if ( *i ) {
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

std::string RDOActivityRuntime::traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index )
{
	std::ostringstream res;
	res << relevantResources.size() << " ";
	for ( std::list< RDOResourceTrace* >::const_iterator i = relevantResources.begin(); i != relevantResources.end(); i++ ) {
#ifdef RDOSIM_COMPATIBLE
		if ( *i && (show_create_index || (!show_create_index && (*i)->getState() != RDOResourceTrace::CS_Create)) ) {
#else
		if ( *i ) {
#endif
			res << " " << (*i)->traceId();
		} else {
			res << " 0";
		}
	}
	return res.str();
}

RDOOperationTrace *RDOActivityOperation::clone2(RDOSimulator *sim) 
{ 
	RDORuntime *runtime = (RDORuntime *)sim;
	RDOActivityOperation *newActivity = new RDOActivityOperation( runtime, pattern, trace, oprName );
	newActivity->paramsCalcs.insert(newActivity->paramsCalcs.begin(), 
		paramsCalcs.begin(), paramsCalcs.end());
	newActivity->relResID.insert(newActivity->relResID.begin(), relResID.begin(), relResID.end());

	return newActivity;
}

void RDOActivityRuntime::writeModelStructure( std::stringstream& stream )
{
	stream << oprName << " " << pattern->getPatternId() << std::endl;
}

void RDOActivityRuntime::incrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( int i = 0; i < relResID.size(); i++ ) {
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( relResID.at(i) );
		if ( res && (res->getState() == RDOResourceTrace::CS_Keep || res->getState() == RDOResourceTrace::CS_Create || res->getState() == RDOResourceTrace::CS_Erase ) ) res->incRef();
	}
}

void RDOActivityRuntime::decrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( int i = 0; i < relResID.size(); i++ ) {
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( relResID.at(i) );
		if ( res && (res->getState() == RDOResourceTrace::CS_Keep || res->getState() == RDOResourceTrace::CS_Create || res->getState() == RDOResourceTrace::CS_Erase ) ) res->decRef();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityRule
// ----------------------------------------------------------------------------
RDOActivityRule::RDOActivityRule( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, const std::string& _oprName ):
	RDORuleTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( NULL )
{
	trace = _trace;
	haveAdditionalCondition = false;
}

RDOActivityRule::RDOActivityRule( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDORuleTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( condition )
{
	trace = _trace;
	haveAdditionalCondition = true;
}

bool RDOActivityRule::choiceFrom( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValueBase( static_cast<RDORuntime*>(sim) ).getBool() ) {
			return false;
		}
	}
	return pattern->choiceFrom( sim ); 
}

void RDOActivityRule::convertRule( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOPATRule*>(pattern)->convertRule( sim ); 
}

void RDOActivityRule::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDORuleTrace::onAfterRule( sim, inSearch );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityIrregEvent
// ----------------------------------------------------------------------------
RDOActivityIrregEvent::RDOActivityIrregEvent( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, const std::string& _oprName ):
	RDOIETrace( rTime, rTime ),
	RDOActivityRuntime( _pattern, _oprName )
{
	trace = _trace;
}

bool RDOActivityIrregEvent::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return pattern->choiceFrom(sim); 
}

void RDOActivityIrregEvent::convertEvent( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOPATIrregEvent*>(pattern)->convertEvent(sim); 
}

void RDOActivityIrregEvent::onBeforeIrregularEvent( RDOSimulator* sim )
{
	setPatternParameters( sim );
	RDOIETrace::onBeforeIrregularEvent( sim );
}

void RDOActivityIrregEvent::onAfterIrregularEvent( RDOSimulator* sim )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDOIETrace::onAfterIrregularEvent( sim );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityIrregEvent::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return static_cast<RDOPATIrregEvent*>(pattern)->getNextTimeInterval(sim); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityOperation
// ----------------------------------------------------------------------------
RDOActivityOperation::RDOActivityOperation( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, const std::string& _oprName ):
	RDOOperationTrace( rTime, rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( NULL )
{
	trace                   = _trace;
	haveAdditionalCondition = false;
}

RDOActivityOperation::RDOActivityOperation( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDOOperationTrace( rTime, rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( condition )
{
	trace                   = _trace;
	haveAdditionalCondition = true;
}

bool RDOActivityOperation::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValueBase( static_cast<RDORuntime*>(sim) ).getBool() ) {
			return false;
		}
	}
	return pattern->choiceFrom( sim ); 
}

void RDOActivityOperation::convertBegin( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOPATOperation*>(pattern)->convertBegin( sim );
}

void RDOActivityOperation::convertEnd( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOPATOperation*>(pattern)->convertEnd( sim );
}

void RDOActivityOperation::onBeforeChoiceFrom( RDOSimulator* sim)
{
	RDOOperationTrace::onBeforeChoiceFrom( sim );
	setPatternParameters( sim );
}

void RDOActivityOperation::onBeforeOperationEnd( RDOSimulator* sim)
{
	setPatternParameters( sim );
	RDOOperationTrace::onBeforeOperationEnd( sim );
}

void RDOActivityOperation::onAfterOperationBegin( RDOSimulator* sim )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDOOperationTrace::onAfterOperationBegin( sim );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	updateConvertStatus( sim, static_cast<RDOPATOperation*>(pattern)->endConvertStatus );
	incrementRelevantResourceReference( sim );
	updateConvertStatus( sim, pattern->beginConvertStatus );
}

void RDOActivityOperation::onAfterOperationEnd( RDOSimulator* sim )
{
	updateConvertStatus( sim, static_cast<RDOPATOperation*>(pattern)->endConvertStatus );
	decrementRelevantResourceReference( sim );
	RDOOperationTrace::onAfterOperationEnd( sim );
	static_cast<RDOPATOperation*>(pattern)->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityOperation::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return ((RDOPATOperation*)pattern)->getNextTimeInterval(sim); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityKeyboard
// ----------------------------------------------------------------------------
RDOActivityKeyboard::RDOActivityKeyboard( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, const std::string& _oprName ):
	RDOActivityOperation( rTime, _pattern, _trace, _oprName ),
	shift( false ),
	control( false ),
	scan_code( -1 )
{
}

RDOActivityKeyboard::RDOActivityKeyboard( RDORuntime* rTime, RDOPattern* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDOActivityOperation( rTime, _pattern, _trace, condition, _oprName ),
	shift( false ),
	control( false ),
	scan_code( -1 )
{
}

RDOActivityRuntime::AddHotKey RDOActivityKeyboard::addHotKey( RDORuntime* runtime, const std::string& hotKey )
{
	unsigned int _scan_code = runtime->rdoHotKeyToolkit.codeFromString( hotKey );
	if ( _scan_code == -1 ) return RDOActivityRuntime::addhk_notfound;
	if ( scan_code != -1 && _scan_code != VK_SHIFT && _scan_code != VK_CONTROL ) return RDOActivityRuntime::addhk_already;
	switch ( _scan_code ) {
		case VK_SHIFT  : shift     = true; runtime->using_scan_codes.push_back( VK_SHIFT   ); break;
		case VK_CONTROL: control   = true; runtime->using_scan_codes.push_back( VK_CONTROL ); break;
		default        : scan_code = _scan_code; if ( scan_code ) runtime->using_scan_codes.push_back( _scan_code ); break;
	}
	return RDOActivityRuntime::addhk_ok;
}

bool RDOActivityKeyboard::choiceFrom( RDOSimulator *sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	runtime->setCurrentActivity( this );

	if ( !runtime->checkAreaActivated( oprName ) ) {
		if ( !runtime->checkKeyPressed( scan_code, shift, control ) ) return false;
	}
	return RDOActivityOperation::choiceFrom( sim ); 
}

} // namespace rdoRuntime
