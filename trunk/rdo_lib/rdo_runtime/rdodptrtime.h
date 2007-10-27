#ifndef RDODTPRTIME_H
#define RDODTPRTIME_H

#include "searchtrace.h"
#include "rdo_runtime.h"
#include "rdocalc.h"
#include <sstream>

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOSearchActivityRuntime - активность внутри DPT (потомок RDOActivity)
// ----------------------------------------------------------------------------
class RDOSearchActivityRuntime: public RDOActivityTrace
{
private:
	RDOCalc* cost;
	virtual double costOfRule( RDOSimulator* runtime ) {
		return cost->calcValueBase( static_cast<RDORuntime*>(runtime) ).getDouble();
	}

public:
	RDOSearchActivityRuntime( RDORuntime* runtime, RDORule* r, bool vA, RDOCalc* _cost ):
		RDOActivityTrace( runtime, r, vA ),
		cost( _cost )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSearchRuntime - DPT (потомок RDODecisionPoint)
// ----------------------------------------------------------------------------
class RDOSearchRuntime: public RDODecisionPointTrace
{
private:
	RDOCalc* condition;
	RDOCalc* termCondition;
	RDOCalc* evaluateBy;
	bool compTops;

	virtual bool Condition( RDOSimulator* sim )     { return condition->calcValueBase( static_cast<RDORuntime*>(sim) ).getBool();     }
	virtual bool TermCondition( RDOSimulator* sim ) { return termCondition->calcValueBase( static_cast<RDORuntime*>(sim) ).getBool(); }
	virtual double EvaluateBy( RDOSimulator* sim )  { return evaluateBy->calcValueBase( static_cast<RDORuntime*>(sim) ).getDouble();  }
	virtual bool NeedCompareTops()                  { return compTops; }

public:
	RDOSearchRuntime( RDORuntime* sim, RDOCalc* _condition, RDOCalc* _termCondition, RDOCalc* _evaluateBy, bool _compTops ):
		RDODecisionPointTrace( sim ),
		condition( _condition ),
		termCondition( _termCondition ),
		evaluateBy( _evaluateBy ),
		compTops( _compTops )
	{
	}
};

} // namespace rdoRuntime

#endif // RDODTPRTIME_H
