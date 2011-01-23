#include "rdo_lib/rdo_converter/pch.h"
#include "rdo_lib/rdo_converter/runtime/rdo_ie.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent( PTR(RDORuntime) rTime, bool trace ):
	RDOPattern( rTime, trace ),
	m_timeCalc( NULL )
{
}

double RDOPatternIrregEvent::getNextTimeInterval( PTR(RDORuntime) runtime )
{
	double time_next = m_timeCalc->calcValue( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next), m_timeCalc );
	return 0;
}

void RDOPatternIrregEvent::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) oprName)
{
	//LPIIrregEvent ie = F(RDOIrregEvent)::create(runtime, this, traceable(), oprName);
	//runtime->addRuntimeIE(parent, ie);
	//return ie;
}

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
RDOIrregEvent::RDOIrregEvent( RDORuntime* runtime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternIrregEvent>( runtime, pattern, trace, name )
{
	setTrace( trace );
	setTraceID( ~0 );
//	setTraceID( runtime->getFreeIrregEventId() );
}

void RDOIrregEvent::onStart( RDOSimulator* sim )
{
	onBeforeIrregularEvent( sim );
	sim->addTimePoint( getNextTimeInterval(sim) + sim->getCurrentTime(), this );
}

void RDOIrregEvent::onStop( RDOSimulator* sim )
{
	sim->removeTimePoint( this );
}

bool RDOIrregEvent::onCheckCondition(RDOSimulator *sim)
{
	return false;
}

IBaseOperation::BOResult RDOIrregEvent::onDoOperation( RDOSimulator* sim )
{
	return IBaseOperation::BOR_cant_run;
}

void RDOIrregEvent::onMakePlaned( RDOSimulator* sim, void* param )
{
	sim->inc_cnt_events();
	onBeforeIrregularEvent( sim );
	convertEvent( sim );
	sim->addTimePoint( getNextTimeInterval(sim) + sim->getCurrentTime(), this );
	onAfterIrregularEvent( sim );
}

void RDOIrregEvent::convertEvent( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEvent( static_cast<RDORuntime*>(sim) ); 
}

void RDOIrregEvent::onBeforeIrregularEvent( RDOSimulator* sim )
{
	setPatternParameters( sim );
}

void RDOIrregEvent::onAfterIrregularEvent( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorStatus );
//	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeIrregularEvent( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOIrregEvent::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

IBaseOperation::BOResult RDOIrregEvent::onContinue(PTR(rdoRuntime::RDOSimulator) sim)
{
	return IBaseOperation::BOR_cant_run;
}

} // namespace rdoRuntime
