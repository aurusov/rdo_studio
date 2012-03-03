#include "pch.h"
#include "rdo_ie.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
RDOIrregEvent::RDOIrregEvent( RDORuntime* runtime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternIrregEvent>( runtime, pattern, trace, name )
{
	setTrace( trace );
	setTraceID( runtime->getFreeIrregEventId() );
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

RDOBaseOperation::BOResult RDOIrregEvent::onDoOperation( RDOSimulator* sim )
{
	return RDOBaseOperation::BOR_cant_run;
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
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeIrregularEvent( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOIrregEvent::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

} // namespace rdoRuntime
