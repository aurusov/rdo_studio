#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoTrace.h"

void RDOTrace::writeSearchBegin(double currentTime, std::string decisionPointId)
{
	if(isNullTracer)
		return;

   getOStream() << "SB " << currentTime << " " << decisionPointId.c_str() << std::endl << getEOL();
}
void RDOTrace::writeSearchDecisionHeader()
{
	if(isNullTracer)
		return;

   getOStream() << "SD" << std::endl << getEOL();
}
void RDOTrace::writeSearchDecision(RDOSimulator *sim, TreeNode *node) 
{
	if(isNullTracer)
		return;

   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   RDOActivityTrace *actTr = (RDOActivityTrace *)node->activity;
   RDORuleTrace *ruleTr = (RDORuleTrace *)actTr->rule;

   getOStream() << node->count
       << " " << actTr->traceId()
       << " " << ruleTr->tracePatternId()
       << " " << traceResourcesListNumbers(simTr, ruleTr->relevantResources)
       << std::endl << getEOL();
}
void RDOTrace::writeString(std::string str)
{
	if(isNullTracer)
		return;

   getOStream() << str << std::endl << getEOL();
}
void RDOTrace::writeSearchOpenNode(int nodeCount, 
   int parentCount,
   double pathCost, 
   double restCost)
{
	if(isNullTracer)
		return;

   getOStream() << "SO " << nodeCount
       << " " << parentCount
       << " " << pathCost
       << " " << restCost << std::endl << getEOL();
}
void RDOTrace::writeSearchNodeInfo(char sign, TreeNodeTrace *node)
{
	if(isNullTracer)
		return;

   RDODecisionPointTrace *dpTrace = (RDODecisionPointTrace *)node->root->dp;
   if(dpTrace->traceFlag == DPT_trace_tops || dpTrace->traceFlag == DPT_trace_all)
   {
      RDOActivityTrace *actTr = (RDOActivityTrace *)node->currAct;
      RDORuleTrace *ruleTr = (RDORuleTrace *)actTr->rule;
      RDOSimulatorTrace *sim = (RDOSimulatorTrace *)node->sim;

		getOStream().precision(4);
      getOStream() << "ST" << sign
         << " " << ((sign != 'D')?node->count:node->root->nodeCount-1)
			<< " " << ((sign != 'D')?node->parent->count:node->count)
         << " " << ((sign != 'D')?node->costPath:node->newCostPath)
         << " " << ((sign != 'D')?node->costRest:node->newCostRest)
         << " " << actTr->traceId()
         << " " << ruleTr->tracePatternId()
         << " " << ((sign != 'D')?node->costRule:node->newCostRule)
         << " " << traceResourcesListNumbers(sim, ruleTr->relevantResources) << std::endl << getEOL();
   
      RDODecisionPointTrace *dpTrace = (RDODecisionPointTrace *)node->root->dp;
      if(dpTrace->traceFlag == DPT_trace_all)
         getOStream() << traceResourcesList('S', sim, ruleTr->relevantResources) << getEOL();
   }
}

void RDOTrace::writeSearchResult(char letter, RDOSimulatorTrace *simTr, TreeRoot *treeRoot)
{
	if(isNullTracer)
		return;

	getOStream() << "SE" << letter 
		<< " " << simTr->getCurrentTime()
		<< " " << 0          // realTime
		<< " " << 0          // memUsed
		<< " " << ((letter == 'S')?treeRoot->targetNode->costPath:0)
		<< " " << treeRoot->expandedNodesCount
		<< " " << treeRoot->nodesInGraphCount
		<< " " << treeRoot->nodeCount - 1
		<< " " << treeRoot->fullNodesCount 
		<< std::endl << getEOL();
}

std::string RDOTrace::traceResourcesListNumbers(RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> resArray)
{
   std::ostringstream res;
   res << resArray.size() << " ";
   for(std::vector<RDOResourceTrace *>::iterator i = resArray.begin(); i != resArray.end(); i++)
	{
		if(*i)
	      res << " " << (*i)->traceId();
		else
	      res << " 0";
	}

   return res.str();
}

void RDOTrace::writePermanentResources(RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> perm)
{
	if(isNullTracer)
		return;

   getOStream() << traceResourcesList('\0', sim, perm) << getEOL();
}

std::string RDOTrace::traceResourcesList(char prefix, RDOSimulatorTrace *sim, std::vector<RDOResourceTrace *> resArray)
{
   std::string res;
   for(std::vector<RDOResourceTrace *>::iterator i = resArray.begin(); i != resArray.end(); i++)
	{
		if(*i)
	      res += (*i)->traceResourceState(prefix, sim);
	}

   return res;
}

std::string RDOResourceTrace::traceResourceState(char prefix, RDOSimulatorTrace *sim)
{
   std::ostringstream res;
   if((trace) || (prefix != '\0'))
   {
      if(prefix != '\0')
         res << prefix;

      res << "R";
      if(justCreated)
         res << "C ";
      else
         res << "K ";

      res << sim->getCurrentTime() << " " << traceTypeId() << " " << traceId()
         << " " << traceParametersValue() << std::endl;
   }
   return res.str();
}

void RDOTrace::writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   getOStream() << traceResourcesList('\0', sim, ie->relevantResources) << getEOL();

   if(ie->trace && canWrite())
   {
      getOStream() << "EI " << sim->getCurrentTime() 
         << " " << ie->traceId() 
         << " " << ie->tracePatternId() 
         << " " << traceResourcesListNumbers(sim, ie->relevantResources)
         << std::endl << getEOL();
   }
}

void RDOTrace::writeRule(RDORuleTrace *rule, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   if(rule->trace)
   {
		int operId = sim->getFreeOperationId();
      getOStream() << "ER " << sim->getCurrentTime()
         << " " << operId
         << " " << rule->traceId() 
         << " " << rule->tracePatternId()
         << " " << traceResourcesListNumbers(sim, rule->relevantResources)
         << std::endl << getEOL();
	   sim->freeOperationId(operId);
   }

   getOStream() << traceResourcesList('\0', sim, rule->relevantResources) << getEOL();
}

void RDOTrace::writeBeforeOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;
}

void RDOTrace::writeAfterOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   if(op->trace)
   {
      getOStream() << "EB " << sim->getCurrentTime() 
         << " " << op->traceOperId() 
         << " " << op->traceId() 
         << " " << op->tracePatternId() 
         << " " << traceResourcesListNumbers(sim, op->relevantResources) << std::endl << getEOL(); 
   }

   getOStream() << traceResourcesList('\0', sim, op->relevantResources) << getEOL();
}

void RDOTrace::writeBeforeOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   if(op->trace)
   {
      getOStream() << "EF " << sim->getCurrentTime() 
         << " " << op->traceOperId() 
         << " " << op->traceId() 
         << " " << op->tracePatternId() 
         << " " << traceResourcesListNumbers(sim, op->relevantResources)
         << std::endl << getEOL();
   }
}

void RDOTrace::writeAfterOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

// check if we delete some of relevant resources:
   for(std::vector<RDOResourceTrace *>::iterator i = op->relevantResources.begin(); i != op->relevantResources.end(); i++)
   {
      if(std::find(sim->allResourcesInSim.begin(), sim->allResourcesInSim.end(), (*i)) == sim->allResourcesInSim.end())
      {
         i = op->relevantResources.erase(i);
         i--;
      }
   }

   getOStream() << traceResourcesList('\0', sim, op->relevantResources) << getEOL();
}

RDOResourceTrace::RDOResourceTrace(RDOSimulatorTrace *i_sim):
   RDOTraceableObject(i_sim)
{
   id = sim->getFreeResourceId();
   justCreated = false;
   tempotary = false;
   trace = false;
}

RDOResourceTrace::RDOResourceTrace(const RDOResourceTrace &orig):
   RDOTraceableObject(orig.sim)
{
   id = orig.id;
   justCreated = orig.justCreated;
   typeId = orig.typeId;
   tempotary = orig.tempotary;
   trace = orig.trace;
   sim->incrementResourceIdReference(id);
}

RDOResourceTrace::~RDOResourceTrace()
{
   sim->onResourceErase(this);
}

void RDOPattern::onAfter(RDOSimulator *child_sim)
{
   relevantResources = getRelevantResources(child_sim);
   std::for_each(relevantResources.begin(), relevantResources.end(), CheckRelevantResource((RDOSimulatorTrace *)child_sim));
}

void CheckRelevantResource::operator ()(RDOResourceTrace *res)
{
	if(res)
	{
		if(std::find(sim->allResourcesInSim.begin(), sim->allResourcesInSim.end(), res)
			== sim->allResourcesInSim.end())
		{
			sim->allResourcesInSim.push_back(res);
			res->tempotary = true;
			res->justCreated = true;
		}
	}
}

void RDOTrace::writeTraceBegin(RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

bool RDOPokazTrace::tracePokaz()
{
	if(!trace || !wasChanged)
		return false;

	sim->getTracer()->writePokaz(sim, this);
	wasChanged = false;
	return true;
}

void RDOTrace::writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok)
{
	if(isNullTracer)
		return;

	getOStream() << "V  "  << sim->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

