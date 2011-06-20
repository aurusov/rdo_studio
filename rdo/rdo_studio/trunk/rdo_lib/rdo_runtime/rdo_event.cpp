/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_event.cpp
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
RDOEvent::RDOEvent( RDORuntime* runtime, RDOPatternEvent* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternEvent>(pattern, trace, name )
{
	setTrace( trace );
	setTraceID( runtime->getFreeEventId() );
}

void RDOEvent::onStart( RDOSimulator* sim )
{
	onBeforeEvent( sim );
}

void RDOEvent::onStop( RDOSimulator* sim )
{
	sim->removeTimePoint( this );
}

bool RDOEvent::onCheckCondition(RDOSimulator *sim)
{
	return false;
}

IBaseOperation::BOResult RDOEvent::onDoOperation( RDOSimulator* sim )
{
	return IBaseOperation::BOR_cant_run;
}

void RDOEvent::onMakePlaned( RDOSimulator* sim, void* param )
{
	sim->inc_cnt_events();
	onBeforeEvent( sim );
	convertEvent( sim );
	onAfterEvent( sim );
}

void RDOEvent::convertEvent( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEvent( static_cast<RDORuntime*>(sim) ); 
}

void RDOEvent::onBeforeEvent( RDOSimulator* sim )
{
	setPatternParameters( sim );
}

void RDOEvent::onAfterEvent( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorStatus );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeEvent( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

IBaseOperation::BOResult RDOEvent::onContinue(PTR(rdoRuntime::RDOSimulator) sim)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
