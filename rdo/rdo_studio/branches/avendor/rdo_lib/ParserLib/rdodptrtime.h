#ifndef RDO_DTP_RUNTIME
#define RDO_DTP_RUNTIME

using namespace std;
#include "searchtrace.h"

#include "rdoruntime.h"

namespace rdoRuntime 
{

class RDOSearchActivityRuntime: public RDOActivityTrace
{
	RDOCalc *cost;
   double costOfRule(RDOSimulator *sim) { return cost->calcValue(dynamic_cast<RDORuntime *>(sim)); }
public:
   RDOSearchActivityRuntime(RDORuntime *sim, RDORule *r, bool vA, RDOCalc *_cost)
		: RDOActivityTrace(sim, r, vA), cost(_cost) {}
};


class RDOSearchRuntime: public RDODecisionPointTrace
{
	RDOCalc *condition;
	RDOCalc *termCondition;
	RDOCalc *evaluateBy;
	bool compTops;

   bool Condition(RDOSimulator *sim) { return (condition->calcValue(dynamic_cast<RDORuntime *>(sim)) != 0);}
   bool TermCondition(RDOSimulator *sim) { return (termCondition->calcValue(dynamic_cast<RDORuntime *>(sim)) != 0);}
   double EvaluateBy(RDOSimulator *sim) { return evaluateBy->calcValue(dynamic_cast<RDORuntime *>(sim)); }
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
