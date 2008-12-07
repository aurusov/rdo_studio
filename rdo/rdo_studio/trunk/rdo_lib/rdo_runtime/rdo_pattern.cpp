#include "pch.h"
#include "rdo_pattern.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"
#include "rdo_keyboard.h"
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
	double time_next = m_timeCalc->calcValue( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), m_timeCalc );
	return 0;
}

RDOIrregEvent* RDOPatternIrregEvent::createActivity( RDORuntime* runtime, const std::string& oprName )
{
	RDOIrregEvent* ie = new RDOIrregEvent( runtime, this, traceable(), oprName );
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

RDORule* RDOPatternRule::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDORule* rule = new RDORule( runtime, this, traceable(), _oprName );
	runtime->addRuntimeRule( rule );
	return rule;
}

RDORule* RDOPatternRule::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDORule* rule = new RDORule( runtime, this, traceable(), condition, _oprName );
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
	double time_next = m_timeCalc->calcValue( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать окончание операции в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), m_timeCalc );
	return 0;
}

RDOOperation* RDOPatternOperation::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOOperation* oper = new RDOOperation( runtime, this, traceable(), _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOOperation* RDOPatternOperation::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName)
{
	RDOOperation* oper = new RDOOperation( runtime, this, traceable(), condition, _oprName );
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

RDOKeyboard* RDOPatternKeyboard::createActivity( RDORuntime* runtime, const std::string& _oprName )
{
	RDOKeyboard* oper = new RDOKeyboard( runtime, this, traceable(), _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

RDOKeyboard* RDOPatternKeyboard::createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName )
{
	RDOKeyboard* oper = new RDOKeyboard( runtime, this, traceable(), condition, _oprName );
	runtime->addRuntimeOperation( oper );
	return oper;
}

} // namespace rdoRuntime
