#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_action.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOAction
// ----------------------------------------------------------------------------
RDOAction::RDOAction( RDORuntime* runtime, RDOPatternAction* pattern, bool trace, const std::string& name ):
	RDOActivityPattern<RDOPatternAction>( runtime, pattern, trace, name ),
	RDOPatternPrior(),
	additionalCondition( NULL )
{
	setTrace( trace );
	haveAdditionalCondition = false;
	setTraceID( runtime->getFreeActivityId() );
}

RDOAction::RDOAction( RDORuntime* runtime, RDOPatternAction* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOActivityPattern<RDOPatternAction>( runtime, pattern, trace, name ),
	RDOPatternPrior(),
	additionalCondition( condition )
{
	setTrace( trace );
	haveAdditionalCondition = true;
	setTraceID( runtime->getFreeActivityId() );
}

RDOAction::~RDOAction()
{
}

bool RDOAction::onCheckCondition(PTR(RDOSimulator) sim)
{
	// ≈сли операци€ может начатьс€, то создать еЄ клон и поместить его в список
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom(sim);
}

IBaseOperation::BOResult RDOAction::onDoOperation(PTR(RDOSimulator) sim)
{
	LPIAction newAction = F(RDOAction)::create(static_cast<PTR(RDORuntime)>(sim), *this);
	newAction->onBeforeActionBegin( sim );
	newAction->convertBegin( sim );
	sim->addTimePoint( newAction->getNextTimeInterval(sim) + sim->getCurrentTime(), newAction );
	newAction->onAfterActionBegin( sim );
	return IBaseOperation::BOR_planned_and_run;
}

void RDOAction::onMakePlaned( RDOSimulator* sim, void* param )
{
	// ¬ыполн€ем событие конца операции-клона
	sim->inc_cnt_events();
	onBeforeActionEnd( sim );
	convertEnd( sim );
	onAfterActionEnd( sim );
}

bool RDOAction::choiceFrom(PTR(RDOSimulator) sim) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( static_cast<RDORuntime*>(sim) ); 
}

void RDOAction::convertBegin(PTR(RDOSimulator) sim)
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOAction::convertEnd(PTR(RDOSimulator) sim)
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEnd( static_cast<RDORuntime*>(sim) );
}

void RDOAction::onBeforeChoiceFrom( RDOSimulator* sim)
{
	setPatternParameters( sim );
}

void RDOAction::onBeforeActionEnd( RDOSimulator* sim)
{
	setPatternParameters( sim );
}

void RDOAction::onAfterActionBegin(PTR(RDOSimulator) sim)
{
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterActionBegin( this, static_cast<RDOSimulatorTrace*>(sim) );
	m_pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	incrementRelevantResourceReference( sim );
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
}

void RDOAction::onAfterActionEnd(PTR(RDOSimulator) sim)
{
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	decrementRelevantResourceReference( sim );
	static_cast<RDOSimulatorTrace*>(sim)->getTracer()->writeAfterActionEnd( this, static_cast<RDOSimulatorTrace*>(sim) ); 
	static_cast<RDOSimulatorTrace*>(sim)->freeOperationId(m_actionId);
	m_pattern->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOAction::getNextTimeInterval(PTR(RDOSimulator) sim) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

RDOAction::RDOAction(PTR(RDORuntime) runtime, CREF(RDOAction) originForClone)
	: RDOActivityPattern<RDOPatternAction>(runtime, originForClone.m_pattern, originForClone.traceable(), originForClone.m_oprName)
	, additionalCondition(NULL)
{
	setTrace( originForClone.traceable() );
	haveAdditionalCondition = false;
	setTraceID( runtime->getFreeActivityId() );

	m_paramsCalcs.insert(m_paramsCalcs.begin(), originForClone.m_paramsCalcs.begin(), originForClone.m_paramsCalcs.end());
	m_relResID   .insert(m_relResID   .begin(), originForClone.m_relResID   .begin(), originForClone.m_relResID   .end());
	setTraceID(originForClone.getTraceID());
	m_actionId = runtime->getFreeOperationId();
}

tstring RDOAction::traceActionId() const
{
	return rdo::toString(m_actionId);
}

void                     RDOAction::onBeforeActionBegin(PTR(rdoRuntime::RDOSimulator) sim) {}
void                     RDOAction::onStart               (PTR(rdoRuntime::RDOSimulator) sim) {}
void                     RDOAction::onStop                (PTR(rdoRuntime::RDOSimulator) sim) {}
IBaseOperation::BOResult RDOAction::onContinue            (PTR(rdoRuntime::RDOSimulator) sim) { return IBaseOperation::BOR_cant_run; }

} // namespace rdoRuntime
