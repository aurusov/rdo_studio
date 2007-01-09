#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopatrtime.h"
#include "rdoruntime.h"
#include "rdoparser.h"
#include "rdohotkey.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOPatternRuntime
// ----------------------------------------------------------------------------
RDOPatternRuntime::RDOPatternRuntime( RDORuntime* _runtime, bool _trace ):
	runtime( _runtime ),
	trace( _trace )
{
	runtime->addPattern(this); 
}

RDOActivityRuntime* RDOPatternRuntime::createActivity( RDOCalc* condition, const std::string& _oprName )
{
	throw rdoParse::RDOInternalException( "parser internal error 0009" );
}

bool RDOPatternRuntime::choiceFrom( RDOSimulator* sim )
{
	RDORuntime* runtime = (RDORuntime*)sim;
//	runtime->allResourcesChoiced.clear();
	int size = choiceFromCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		if ( !choiceFromCalcs[i]->calcValueBase(runtime) ) return false;
	}
	return true;
}

void RDOPatternRuntime::convertBegin( RDORuntime* _runtime )
{
	int size = beginCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		beginCalcs.at(i)->calcValueBase( _runtime );
	}
}

void RDOPatternRuntime::convertBeginErase( RDORuntime* _runtime )
{
	int size = beginEraseCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		beginEraseCalcs.at(i)->calcValueBase( _runtime );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORuleRuntime
// ----------------------------------------------------------------------------
RDORuleRuntime::RDORuleRuntime( RDORuntime* rTime, bool _trace ):
	RDOPatternRuntime( rTime, _trace )
{
}

void RDORuleRuntime::convertRule( RDOSimulator* sim )
{
	RDOPatternRuntime::convertBegin( static_cast<RDORuntime*>(sim) );
}

RDOActivityRuntime* RDORuleRuntime::createActivity( const std::string& _oprName )
{
	RDOActivityRuleRuntime* rule = new RDOActivityRuleRuntime( runtime, this, trace, _oprName );
	rdoParse::parser->runTime->addRuntimeRule( rule );
	return rule;
}

RDOActivityRuntime* RDORuleRuntime::createActivity( RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityRuleRuntime* rule = new RDOActivityRuleRuntime( runtime, this, trace, condition, _oprName );
	rdoParse::parser->runTime->addRuntimeRule( rule );
	return rule;
}

// ----------------------------------------------------------------------------
// ---------- RDOIERuntime
// ----------------------------------------------------------------------------
RDOIERuntime::RDOIERuntime( RDORuntime* rTime, bool _trace ):
	RDOPatternRuntime( rTime, _trace ),
	timeCalc( NULL )
{
}

double RDOIERuntime::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeCalc->calcValueBase( runtime );
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next).c_str(), timeCalc );
	return 0;
}

void RDOIERuntime::convertEvent( RDOSimulator* sim )
{
	choiceFrom( sim ); // to set permanent resource relevent numbers
	RDOPatternRuntime::convertBegin( static_cast<RDORuntime*>(sim) );
}

RDOActivityRuntime* RDOIERuntime::createActivity( const std::string& _oprName )
{
	RDOActivityIERuntime* ie = new RDOActivityIERuntime( runtime, this, trace, _oprName );
	rdoParse::parser->runTime->addRuntimeIE( ie );
	return ie;
}

// ----------------------------------------------------------------------------
// ---------- RDOOperationRuntime
// ----------------------------------------------------------------------------
RDOOperationRuntime::RDOOperationRuntime( RDORuntime* rTime, bool _trace ):
	RDOPatternRuntime( rTime, _trace ),
	timeCalc( NULL )
{
}

double RDOOperationRuntime::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeCalc->calcValueBase( runtime );
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать окончание операции в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next).c_str(), timeCalc );
	return 0;
}

void RDOOperationRuntime::convertBegin( RDOSimulator* sim )
{
	RDOPatternRuntime::convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOOperationRuntime::convertEnd( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = endCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		endCalcs.at(i)->calcValueBase( runtime );
	}
}

void RDOOperationRuntime::convertEndErase( RDORuntime* _runtime )
{
	int size = endEraseCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		endEraseCalcs.at(i)->calcValueBase( _runtime );
	}
}

RDOActivityRuntime* RDOOperationRuntime::createActivity( const std::string& _oprName )
{
	RDOActivityOperationRuntime* oper = new RDOActivityOperationRuntime( runtime, this, trace, _oprName );
	rdoParse::parser->runTime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityRuntime* RDOOperationRuntime::createActivity( RDOCalc* condition, const std::string& _oprName)
{
	RDOActivityOperationRuntime* oper = new RDOActivityOperationRuntime( runtime, this, trace, condition, _oprName );
	rdoParse::parser->runTime->addRuntimeOperation( oper );
	return oper;
}

// ----------------------------------------------------------------------------
// ---------- RDOKeyboardRuntime
// ----------------------------------------------------------------------------
RDOKeyboardRuntime::RDOKeyboardRuntime( RDORuntime* rTime, bool _trace ):
	RDOOperationRuntime( rTime, _trace )
{
}

RDOActivityRuntime* RDOKeyboardRuntime::createActivity( const std::string& _oprName )
{
	RDOActivityKeyboardRuntime* oper = new RDOActivityKeyboardRuntime( runtime, this, trace, _oprName );
	rdoParse::parser->runTime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityRuntime* RDOKeyboardRuntime::createActivity( RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityKeyboardRuntime* oper = new RDOActivityKeyboardRuntime( runtime, this, trace, condition, _oprName );
	rdoParse::parser->runTime->addRuntimeOperation( oper );
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
		paramsCalcs.at(i)->calcValueBase(runtime);
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
//					static_cast<RDORuntime*>(sim)->allResourcesInSim.push_back( res );
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

std::string RDOActivityRuntime::traceResourcesListNumbers( RDOSimulatorTrace* sim )
{
	std::ostringstream res;
	res << relevantResources.size();
	for ( std::list< RDOResourceTrace* >::const_iterator i = relevantResources.begin(); i != relevantResources.end(); i++ ) {
		if ( *i && (*i)->getState() != RDOResourceTrace::CS_Create ) {
			res << " " << (*i)->traceId();
		} else {
			res << " 0";
		}
	}
	return res.str();
}

void RDOActivityRuntime::addHotKey( std::string* hotKey )
{
	rdoParse::parser->error( "ќбразец не €вл€етс€ клавиатурной операцией" );
}

RDOOperationTrace *RDOActivityOperationRuntime::clone2(RDOSimulator *sim) 
{ 
	RDORuntime *runtime = (RDORuntime *)sim;
	RDOActivityOperationRuntime *newActivity = new RDOActivityOperationRuntime( runtime, pattern, trace, oprName );
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
		if ( res ) res->referenceCount++;
	}
}

void RDOActivityRuntime::decrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( int i = 0; i < relResID.size(); i++ ) {
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( relResID.at(i) );
		if ( res ) res->referenceCount--;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityRuleRuntime
// ----------------------------------------------------------------------------
RDOActivityRuleRuntime::RDOActivityRuleRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName ):
	RDORuleTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( NULL )
{
	trace = _trace;
	haveAdditionalCondition = false;
}

RDOActivityRuleRuntime::RDOActivityRuleRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDORuleTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( condition )
{
	trace = _trace;
	haveAdditionalCondition = true;
}

bool RDOActivityRuleRuntime::choiceFrom(RDOSimulator *sim) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity(this);
	if(haveAdditionalCondition)
		if(!additionalCondition->calcValueBase(dynamic_cast<RDORuntime *>(sim)))
			return false;

	return pattern->choiceFrom( sim ); 
}

void RDOActivityRuleRuntime::convertRule( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDORuleRuntime*>(pattern)->convertRule( sim ); 
}

void RDOActivityRuleRuntime::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDORuleTrace::onAfterRule( sim, inSearch );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityIERuntime
// ----------------------------------------------------------------------------
RDOActivityIERuntime::RDOActivityIERuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName ):
	RDOIETrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName )
{
	trace = _trace;
}

bool RDOActivityIERuntime::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return pattern->choiceFrom(sim); 
}

void RDOActivityIERuntime::convertEvent( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOIERuntime*>(pattern)->convertEvent(sim); 
}

void RDOActivityIERuntime::onBeforeIrregularEvent( RDOSimulator* sim )
{
	setPatternParameters( sim );
	RDOIETrace::onBeforeIrregularEvent( sim );
}

void RDOActivityIERuntime::onAfterIrregularEvent( RDOSimulator* sim )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDOIETrace::onAfterIrregularEvent( sim );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityIERuntime::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return static_cast<RDOIERuntime*>(pattern)->getNextTimeInterval(sim); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityOperationRuntime
// ----------------------------------------------------------------------------
RDOActivityOperationRuntime::RDOActivityOperationRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName ):
	RDOOperationTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( NULL )
{
	trace = _trace;
	haveAdditionalCondition = false;
}

RDOActivityOperationRuntime::RDOActivityOperationRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDOOperationTrace( rTime ),
	RDOActivityRuntime( _pattern, _oprName ),
	additionalCondition( condition )
{
	trace = _trace;
	haveAdditionalCondition = true;
	
}

bool RDOActivityOperationRuntime::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast< RDORuntime* >(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValueBase( dynamic_cast<RDORuntime *>(sim) ) ) {
			return false;
		}
	}
	return pattern->choiceFrom( sim ); 
}

void RDOActivityOperationRuntime::convertBegin( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOOperationRuntime*>(pattern)->convertBegin( sim );
}

void RDOActivityOperationRuntime::convertEnd( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	static_cast<RDOOperationRuntime*>(pattern)->convertEnd( sim );
}

void RDOActivityOperationRuntime::onBeforeChoiceFrom( RDOSimulator* sim)
{
	RDOOperationTrace::onBeforeChoiceFrom( sim );
	setPatternParameters( sim );
}

void RDOActivityOperationRuntime::onBeforeOperationEnd( RDOSimulator* sim)
{
	decrementRelevantResourceReference( sim );
	setPatternParameters( sim );
	RDOOperationTrace::onBeforeOperationEnd( sim );
}

void RDOActivityOperationRuntime::onAfterOperationBegin( RDOSimulator* sim )
{
	updateConvertStatus( sim, pattern->beginConvertStatus );
	RDOOperationTrace::onAfterOperationBegin( sim );
	pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	incrementRelevantResourceReference( sim );
}

void RDOActivityOperationRuntime::onAfterOperationEnd( RDOSimulator* sim )
{
	updateConvertStatus( sim, static_cast<RDOOperationRuntime*>(pattern)->endConvertStatus );
	RDOOperationTrace::onAfterOperationEnd( sim );
	static_cast<RDOOperationRuntime*>(pattern)->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityOperationRuntime::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return ((RDOOperationRuntime*)pattern)->getNextTimeInterval(sim); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityKeyboardRuntime
// ----------------------------------------------------------------------------
RDOActivityKeyboardRuntime::RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName ):
	RDOActivityOperationRuntime( rTime, _pattern, _trace, _oprName ),
	shift( false ),
	control( false ),
	scan_code( 0 )
{
}

RDOActivityKeyboardRuntime::RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName ):
	RDOActivityOperationRuntime( rTime, _pattern, _trace, condition, _oprName ),
	shift( false ),
	control( false ),
	scan_code( 0 )
{
}

void RDOActivityKeyboardRuntime::addHotKey( std::string* hotKey )
{
	unsigned int _scan_code = rdoParse::rdoHotKeyToolkit.codeFromString( hotKey );
	switch ( _scan_code ) {
		case VK_SHIFT  : shift     = true;       rdoParse::parser->runTime->using_scan_codes.push_back( VK_SHIFT   ); break;
		case VK_CONTROL: control   = true;       rdoParse::parser->runTime->using_scan_codes.push_back( VK_CONTROL ); break;
		default        : scan_code = _scan_code; rdoParse::parser->runTime->using_scan_codes.push_back( _scan_code ); break;
	}
}

bool RDOActivityKeyboardRuntime::choiceFrom( RDOSimulator *sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	runtime->setCurrentActivity( this );

	if ( !runtime->checkAreaActivated( oprName ) ) {
		if ( !runtime->checkKeyPressed( scan_code, shift, control ) ) return false;
	}
	return RDOActivityOperationRuntime::choiceFrom( sim ); 
}

} // namespace rdoRuntime
