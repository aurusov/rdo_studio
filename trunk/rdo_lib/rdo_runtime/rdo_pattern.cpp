#include "pch.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"
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
RDOPattern::RDOPattern( RDORuntime* runtime, bool trace ):
	RDORuntimeParent( runtime ),
	RDOTraceableObject( trace )
{
}

/*
bool RDOPattern::choiceFrom( RDOSimulator* sim )
{
	RDORuntime* runtime = (RDORuntime*)sim;
	int size = m_choiceFromCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		if ( !m_choiceFromCalcs[i]->calcValueBase( runtime ).getBool() ) return false;
	}
	return true;
}

void RDOPattern::convertBegin( RDORuntime* _runtime )
{
	int size = m_beginCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		m_beginCalcs.at(i)->calcValueBase( _runtime );
	}
}

void RDOPattern::convertBeginErase( RDORuntime* _runtime )
{
	int size = m_beginEraseCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		m_beginEraseCalcs.at(i)->calcValueBase( _runtime );
	}
}
*/
// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent( RDORuntime* rTime, bool trace ):
	RDOPattern( rTime, trace ),
	m_timeCalc( NULL )
{
}

double RDOPatternIrregEvent::getNextTimeInterval( RDORuntime* runtime )
{
	double time_next = m_timeCalc->calcValueBase( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), m_timeCalc );
	return 0;
}

RDOActivityIrregEvent* RDOPatternIrregEvent::createActivity( RDORuntime* runtime, const std::string& oprName )
{
	RDOActivityIrregEvent* ie = new RDOActivityIrregEvent( runtime, this, traceable(), oprName );
	runtime->addRuntimeIE( ie );
	return ie;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule( RDORuntime* rTime, bool trace ):
	RDOPattern( rTime, trace )
{
}

RDOActivityRule* RDOPatternRule::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityRule* rule = new RDOActivityRule( runtime, this, traceable(), _oprName );
	runtime->addRuntimeRule( rule );
	return rule;
}

RDOActivityRule* RDOPatternRule::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityRule* rule = new RDOActivityRule( runtime, this, traceable(), condition, _oprName );
	runtime->addRuntimeRule( rule );
	return rule;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation( RDORuntime* rTime, bool trace ):
	RDOPattern( rTime, trace ),
	m_timeCalc( NULL )
{
}

double RDOPatternOperation::getNextTimeInterval( RDORuntime* runtime )
{
	double time_next = m_timeCalc->calcValueBase( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать окончание операции в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), m_timeCalc );
	return 0;
}

RDOActivityOperation* RDOPatternOperation::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityOperation* oper = new RDOActivityOperation( runtime, this, traceable(), _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityOperation* RDOPatternOperation::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName)
{
	RDOActivityOperation* oper = new RDOActivityOperation( runtime, this, traceable(), condition, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard( RDORuntime* rTime, bool _trace ):
	RDOPatternOperation( rTime, _trace )
{
}

RDOActivityKeyboard* RDOPatternKeyboard::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOActivityKeyboard* oper = new RDOActivityKeyboard( runtime, this, traceable(), _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOActivityKeyboard* RDOPatternKeyboard::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDOActivityKeyboard* oper = new RDOActivityKeyboard( runtime, this, traceable(), condition, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

} // namespace rdoRuntime
