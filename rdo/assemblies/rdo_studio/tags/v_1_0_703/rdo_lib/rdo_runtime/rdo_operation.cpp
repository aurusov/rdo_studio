#include "pch.h"
#include "rdo_operation.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
RDOOperation::RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternOperation>( runtime, pattern, trace, name ),
	m_clones( NULL ),
	additionalCondition( NULL )
{
	setTrace( trace );
	haveAdditionalCondition = false;
	setTraceID( runtime->getFreeActivityId() );
	m_clones.reparent( this );
}

RDOOperation::RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOActivityPattern<RDOPatternOperation>( runtime, pattern, trace, name ),
	m_clones( NULL ),
	additionalCondition( condition )
{
	setTrace( trace );
	haveAdditionalCondition = true;
	setTraceID( runtime->getFreeActivityId() );
	m_clones.reparent( this );
}

RDOOperation::~RDOOperation()
{
	m_clones.deleteObjects();
}

bool RDOOperation::onCheckCondition( RDOSimulator* sim )
{
	// ≈сли операци€ может начатьс€, то создать еЄ клон и поместить его в список
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom(sim);
}

RDOBaseOperation::BOResult RDOOperation::onDoOperation( RDOSimulator* sim )
{
	RDOOperation* newOp = clone( sim );
	newOp->reparent( &m_clones );
	newOp->onBeforeOperationBegin( sim );
	newOp->convertBegin( sim );
	sim->addTimePoint( newOp->getNextTimeInterval(sim) + sim->getCurrentTime(), this, newOp );
	newOp->onAfterOperationBegin( sim );
	return RDOBaseOperation::BOR_planned_and_run;
}

void RDOOperation::onMakePlaned( RDOSimulator* sim, void* param )
{
	// ¬ыполн€ем событие конца операции-клона
	sim->inc_cnt_events();
	RDOOperation* opr = static_cast<RDOOperation*>(param);
	opr->onBeforeOperationEnd( sim );
	opr->convertEnd( sim );
	opr->onAfterOperationEnd( sim );
	delete opr;
}

bool RDOOperation::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( static_cast<RDORuntime*>(sim) ); 
}

void RDOOperation::convertBegin( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOOperation::convertEnd( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEnd( static_cast<RDORuntime*>(sim) );
}

void RDOOperation::onBeforeChoiceFrom( RDOSimulator* sim)
{
	setPatternParameters( sim );
}

void RDOOperation::onBeforeOperationEnd( RDOSimulator* sim)
{
	setPatternParameters( sim );
}

void RDOOperation::onAfterOperationBegin( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterOperationBegin( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	incrementRelevantResourceReference( sim );
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
}

void RDOOperation::onAfterOperationEnd( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	decrementRelevantResourceReference( sim );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterOperationEnd( this, static_cast<RDOSimulatorTrace*>(sim) ); 
	static_cast<RDOSimulatorTrace*>(sim)->freeOperationId(m_operId);
	m_pattern->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOOperation::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

} // namespace rdoRuntime
