#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoTrace.h"

void RDOSimulatorTrace::addTemplateDecisionPoint(RDODecisionPointTrace *dp) 
{ 
   dp->id = dpCounter++;
//   dp->id = haveDecisionPoints.size() + 1;
//   RDOSimulator::addTemplateDecisionPoint(dp);
   RDOSimulator::addTemplateBaseOperation(dp);
}

void RDOSimulatorTrace::addTemplateOperation(RDOOperationTrace *op) 
{ 
   op->id = operationCounter++;
//   freeOperationId(op->operId);
//   RDOSimulator::addTemplateOperation(op); 
   RDOSimulator::addTemplateBaseOperation(op); 
}

void RDOSimulatorTrace::addTemplateIrregularEvent(RDOIETrace *ev)
{
   ev->id = ieCounter++;
//   ev->id = haveIrregularEvents.size() + 1;
//   RDOSimulator::addTemplateIrregularEvent(ev); 
   RDOSimulator::addTemplateBaseOperation(ev); 
}

void RDOSimulatorTrace::addTemplateRule(RDORuleTrace *rule)
{
   rule->id = operationCounter++;
//   RDOSimulator::addTemplateRule(rule); 
   RDOSimulator::addTemplateBaseOperation(rule); 
}

void RDOSimulatorTrace::addTemplateBaseOperation(RDOBaseOperation *bop)
{
	RDORuleTrace *rule = dynamic_cast<RDORuleTrace *>(bop);
	RDOIETrace *ie = dynamic_cast<RDOIETrace *>(bop);
	RDOOperationTrace *op = dynamic_cast<RDOOperationTrace *>(bop);
	RDODecisionPointTrace *dp = dynamic_cast<RDODecisionPointTrace *>(bop);
	if(rule) addTemplateRule(rule);
	else if(ie) addTemplateIrregularEvent(ie);
	else if(op) addTemplateOperation(op);
	else if(dp) addTemplateDecisionPoint(dp);
}

void RDOSimulatorTrace::rdoDestroy()
{
   RDOSimulator::rdoDestroy();
}

void RDOSimulatorTrace::rdoInit()
{
   maxResourcesId = 1;
   maxOperationId = 1;
   RDOSimulator::rdoInit();
}

int RDOSimulatorTrace::getFreeResourceId()
{
   if(freeResourcesIds.empty())
      return maxResourcesId++;
   else
   {
      int id = freeResourcesIds.front();
      freeResourcesIds.pop_front();
      return id;
   }
}

void RDOSimulatorTrace::freeResourceId(int id) 
{
   MAPII::iterator it = resourcesIdsRefs.find(id);
   if(it != resourcesIdsRefs.end())
   {
      if(--(*it).second >= 1)
         return;

      resourcesIdsRefs.erase(it);
   }

   freeResourcesIds.push_front(id); 
}

void RDOSimulatorTrace::incrementResourceIdReference(int id)
{
   MAPII::iterator it = resourcesIdsRefs.find(id);
   if(it == resourcesIdsRefs.end())
      resourcesIdsRefs.insert(MAPII::value_type(id, 2));
   else
      (*it).second ++;
}

int RDOSimulatorTrace::getFreeOperationId() 
{ 
   if(freeOperationsIds.empty())
      return maxOperationId++;
   else
   {
      int id = freeOperationsIds.front();
      freeOperationsIds.pop_front();
      return id;
   }
}

void RDOSimulatorTrace::freeOperationId(int id) 
{ 
   freeOperationsIds.push_front(id); 
}

void RDOSimulatorTrace::onResourceErase(RDOResourceTrace *res)
{
   if(res->tempotary && res->trace && getTracer()->canWrite())
   {
      getTracer()->getOStream() << "RE "
         << getCurrentTime() << " " 
         << res->typeId.c_str() << " " 
         << res->traceId().c_str() << std::endl << getTracer()->getEOL();
   }
   allResourcesInSim.remove(res);
   freeResourceId(res->id);
}

void RDOSimulatorTrace::clearJustCreatedFlags()
{
   std::for_each(allResourcesInSim.begin(), allResourcesInSim.end(), RDOResourceTrace::clearJCFlag);
}

void RDOSimulatorTrace::checkPermanentResources()
{
   perm = getPermanentResources();
   allResourcesInSim.clear();
   allResourcesInSim.insert(allResourcesInSim.begin(), perm.begin(), perm.end());
}

void RDOSimulatorTrace::preProcess()
{
   RDOSimulator::preProcess();
	getTracer()->startWriting();
   getTracer()->writeTraceBegin(this);
   checkPermanentResources();
   getTracer()->writePermanentResources(this, perm);
   getTracer()->writeModelBegin(this);
}

void RDOSimulatorTrace::postProcess()
{
   RDOSimulator::postProcess();
	getTracer()->stopWriting();
}

RDOSimulator *RDOSimulatorTrace::createCopy()
{
   RDOSimulator *res = RDOSimulator::createCopy();
   ((RDOSimulatorTrace*)res)->checkPermanentResources();
   return res;
}

		// TEMPLATE CLASS caster
template<class FromType, class _Ufn>
class caster
{
public:
	caster(const _Ufn& _X)
		: op(_X) {}
	_Ufn::result_type operator()(const FromType& _X) const
	{ return op((_Ufn::argument_type)_X); }
protected:
	_Ufn op;
};
		// TEMPLATE FUNCTION cast
template<class FromType, class _Ufn> inline
caster<FromType, _Ufn> cast(const _Ufn& _X)
{
	return caster<FromType, _Ufn>(_X); 
}




void RDOSimulatorTrace::onAfterCheckPokaz()
{
	std::for_each(havePokaz.rbegin(), havePokaz.rend(), cast<RDOPokaz *>(std::mem_fun(&RDOPokazTrace::tracePokaz)));
}
