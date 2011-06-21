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
RDOEvent::RDOEvent( CREF(LPRDORuntime) pRuntime, RDOPatternEvent* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternEvent>(pattern, trace, name )
{
	setTrace( trace );
	setTraceID( pRuntime->getFreeEventId() );
}

void RDOEvent::onStart( CREF(LPRDORuntime) pRuntime )
{
	onBeforeEvent( pRuntime );
}

void RDOEvent::onStop( CREF(LPRDORuntime) pRuntime )
{
	pRuntime->removeTimePoint( this );
}

bool RDOEvent::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	return false;
}

IBaseOperation::BOResult RDOEvent::onDoOperation( CREF(LPRDORuntime) pRuntime )
{
	return IBaseOperation::BOR_cant_run;
}

void RDOEvent::onMakePlaned( CREF(LPRDORuntime) pRuntime, void* param )
{
	pRuntime->inc_cnt_events();
	onBeforeEvent( pRuntime );
	convertEvent( pRuntime );
	onAfterEvent( pRuntime );
}

void RDOEvent::convertEvent( CREF(LPRDORuntime) pRuntime ) 
{ 
	pRuntime->setCurrentActivity(this);
	m_pattern->convertEvent(pRuntime); 
}

void RDOEvent::onBeforeEvent( CREF(LPRDORuntime) pRuntime )
{
	setPatternParameters( pRuntime );
}

void RDOEvent::onAfterEvent( CREF(LPRDORuntime) pRuntime )
{
	updateConvertStatus( pRuntime, m_pattern->m_convertorStatus );
	pRuntime->getTracer()->writeEvent( this, pRuntime );
	m_pattern->convertErase( pRuntime );
	updateRelRes( pRuntime );
}

IBaseOperation::BOResult RDOEvent::onContinue(CREF(LPRDORuntime) pRuntime)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
