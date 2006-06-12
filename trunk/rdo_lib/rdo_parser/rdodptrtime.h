#ifndef RDO_DTP_RUNTIME
#define RDO_DTP_RUNTIME

#include <searchtrace.h>

#include "rdoruntime.h"
#include <sstream>

namespace rdoRuntime 
{

class RDOSearchActivityRuntime: public RDOActivityTrace
{
private:
	RDOCalc *cost;
	double costOfRule(RDOSimulator *sim) { return cost->calcValueBase(dynamic_cast<RDORuntime *>(sim)); }
public:
	RDOSearchActivityRuntime(RDORuntime *sim, RDORule *r, bool vA, RDOCalc *_cost): RDOActivityTrace(sim, r, vA), cost(_cost) {}
};


class RDOSearchRuntime: public RDODecisionPointTrace
{
private:
	RDOCalc *condition;
	RDOCalc *termCondition;
	RDOCalc *evaluateBy;
	bool compTops;

	bool Condition(RDOSimulator *sim) { return (condition->calcValueBase(dynamic_cast<RDORuntime *>(sim)) != 0);}
	bool TermCondition(RDOSimulator *sim) { return (termCondition->calcValueBase(dynamic_cast<RDORuntime *>(sim)) != 0);}
	double EvaluateBy(RDOSimulator *sim) { return evaluateBy->calcValueBase(dynamic_cast<RDORuntime *>(sim)); }
	bool NeedCompareTops() { return compTops; }

public:
	RDOSearchRuntime(RDORuntime *sim, 
		RDOCalc *_condition, 
		RDOCalc *_termCondition, 
		RDOCalc *_evaluateBy, 
		bool _compTops)
			: RDODecisionPointTrace(sim),
			condition(_condition),
			termCondition(_termCondition),
			evaluateBy(_evaluateBy),
			compTops(_compTops)
		{}
};


}

#endif //RDO_DTP_RUNTIME
