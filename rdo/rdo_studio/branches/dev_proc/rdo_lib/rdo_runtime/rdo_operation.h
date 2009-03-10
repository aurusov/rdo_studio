#ifndef RDO_OPERATION_H
#define RDO_OPERATION_H

#include "rdo.h"
#include "rdotrace.h"
#include "simtrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
class RDOOperation: public RDOActivityPattern<RDOPatternOperation>
{
friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

public:
	RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, const std::string& name );
	RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name );
	virtual ~RDOOperation();

	        void onBeforeChoiceFrom    ( RDOSimulator* sim );
	virtual bool choiceFrom            ( RDOSimulator* sim );
	        void onBeforeOperationBegin( RDOSimulator* sim )  {}
	        void convertBegin          ( RDOSimulator* sim );
	        void onAfterOperationBegin ( RDOSimulator* sim );
	        void onBeforeOperationEnd  ( RDOSimulator* sim );
	        void convertEnd            ( RDOSimulator* sim );
	        void onAfterOperationEnd   ( RDOSimulator* sim );

private:
	RDORuntimeParent m_clones;

	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
	virtual void     onMakePlaned    ( RDOSimulator* sim, void* param = NULL );

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	double getNextTimeInterval(RDOSimulator *sim);

	std::string traceOperId() { return rdo::toString(m_operId); }

	int m_operId;

	RDOOperation* clone( RDOSimulator* sim )
	{
		RDORuntime* runtime = (RDORuntime *)sim;
		RDOOperation* newOper = new RDOOperation( runtime, m_pattern, traceable(), m_oprName );
		newOper->m_paramsCalcs.insert(newOper->m_paramsCalcs.begin(), m_paramsCalcs.begin(), m_paramsCalcs.end());
		newOper->m_relResID.insert(newOper->m_relResID.begin(), m_relResID.begin(), m_relResID.end());
		newOper->setTraceID( getTraceID() );
		newOper->m_operId = static_cast<RDOSimulatorTrace*>(sim)->getFreeOperationId();
		return newOper;
	};
};

} // namespace rdoRuntime

#endif // RDO_OPERATION_H
