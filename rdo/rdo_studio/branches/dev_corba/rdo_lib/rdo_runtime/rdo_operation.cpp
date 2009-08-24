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
	RDOPatternPrior(),
	additionalCondition( NULL )
{
	setTrace( trace );
	haveAdditionalCondition = false;
	setTraceID( runtime->getFreeActivityId() );
}

RDOOperation::RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOActivityPattern<RDOPatternOperation>( runtime, pattern, trace, name ),
	RDOPatternPrior(),
	additionalCondition( condition )
{
	setTrace( trace );
	haveAdditionalCondition = true;
	setTraceID( runtime->getFreeActivityId() );
}

RDOOperation::~RDOOperation()
{
}

bool RDOOperation::onCheckCondition(PTR(RDOSimulator) sim)
{
	// ≈сли операци€ может начатьс€, то создать еЄ клон и поместить его в список
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom(sim);
}

IBaseOperation::BOResult RDOOperation::onDoOperation(PTR(RDOSimulator) sim)
{
	LPIOperation newOper = F(RDOOperation)::create(static_cast<PTR(RDORuntime)>(sim), *this);
	newOper->onBeforeOperationBegin( sim );
	newOper->convertBegin( sim );
	sim->addTimePoint( newOper->getNextTimeInterval(sim) + sim->getCurrentTime(), newOper );
	newOper->onAfterOperationBegin( sim );
	return IBaseOperation::BOR_planned_and_run;
}

void RDOOperation::onMakePlaned( RDOSimulator* sim, void* param )
{
	// ¬ыполн€ем событие конца операции-клона
	sim->inc_cnt_events();
	onBeforeOperationEnd( sim );
	convertEnd( sim );
	onAfterOperationEnd( sim );
}

bool RDOOperation::choiceFrom(PTR(RDOSimulator) sim) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( static_cast<RDORuntime*>(sim) ); 
}

void RDOOperation::convertBegin(PTR(RDOSimulator) sim)
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOOperation::convertEnd(PTR(RDOSimulator) sim)
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

void RDOOperation::onAfterOperationBegin(PTR(RDOSimulator) sim)
{
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterOperationBegin( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	incrementRelevantResourceReference( sim );
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
}

void RDOOperation::onAfterOperationEnd(PTR(RDOSimulator) sim)
{
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	decrementRelevantResourceReference( sim );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterOperationEnd( this, static_cast<RDOSimulatorTrace*>(sim) ); 
	static_cast<RDOSimulatorTrace*>(sim)->freeOperationId(m_operId);
	m_pattern->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOOperation::getNextTimeInterval(PTR(RDOSimulator) sim) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

RDOOperation::RDOOperation(PTR(RDORuntime) runtime, CREF(RDOOperation) originForClone)
	: RDOActivityPattern<RDOPatternOperation>(runtime, originForClone.m_pattern, originForClone.traceable(), originForClone.m_oprName)
	, additionalCondition(NULL)
{
	setTrace( originForClone.traceable() );
	haveAdditionalCondition = false;
	setTraceID( runtime->getFreeActivityId() );

	m_paramsCalcs.insert(m_paramsCalcs.begin(), originForClone.m_paramsCalcs.begin(), originForClone.m_paramsCalcs.end());
	m_relResID   .insert(m_relResID   .begin(), originForClone.m_relResID   .begin(), originForClone.m_relResID   .end());
	setTraceID(originForClone.getTraceID());
	m_operId = runtime->getFreeOperationId();
}

tstring RDOOperation::traceOperId() const
{
	return rdo::toString(m_operId);
}

void                     RDOOperation::onBeforeOperationBegin(PTR(rdoRuntime::RDOSimulator) sim) {}
void                     RDOOperation::onStart               (PTR(rdoRuntime::RDOSimulator) sim) {}
void                     RDOOperation::onStop                (PTR(rdoRuntime::RDOSimulator) sim) {}
IBaseOperation::BOResult RDOOperation::onContinue            (PTR(rdoRuntime::RDOSimulator) sim) { return IBaseOperation::BOR_cant_run; }

} // namespace rdoRuntime
