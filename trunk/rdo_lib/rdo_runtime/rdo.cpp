#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

#include "rdo.h"

bool CheckOperations::operator()(RDOBaseOperation *op)
{
	return op->checkOperation(sim);
}

bool RDODecisionPoint::checkOperation(RDOSimulator *sim)
{
   if(Condition(sim))
   {
      if(RunSearchInTree(sim))
         return true;
   }
   return false;
}

bool RDOOperation::checkOperation(RDOSimulator *sim)
{
   onBeforeChoiceFrom(sim);
   if(!choiceFrom(sim))
      return false;

   RDOOperation *newOp = clone(sim);
   newOp->onBeforeOperationBegin(sim);
   newOp->convertBegin(sim);
   sim->addOperation(newOp);
   sim->addTimePoint(newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()));
   newOp->onAfterOperationBegin(sim);
   return true;
}

bool RDOIE::checkOperation(RDOSimulator *sim)
{
   if(sim->getCurrentTime() >= time)
   {
      onBeforeIrregularEvent(sim);
      convertEvent(sim);
      sim->addTimePoint(time = (getNextTimeInterval(sim) + sim->getCurrentTime()));
      onAfterIrregularEvent(sim);
      return true;
   }

   return false;
}

bool RDORule::checkOperation(RDOSimulator *sim)
{
   onBeforeChoiceFrom(sim);
   if(choiceFrom(sim))
   {
      onBeforeRule(sim);
      convertRule(sim);
      onAfterRule(sim);
      return true;
   }
   return false;
}

/*
bool CheckOperations::operator()(RDODecisionPoint *dp)
{
   if(dp->Condition(sim))
   {
      if(dp->RunSearchInTree(sim))
         return true;
   }
   return false;
}

bool CheckOperations::operator()(RDOOperation *op)
{
   op->onBeforeChoiceFrom(sim);
   if(!op->choiceFrom(sim))
      return false;

   RDOOperation *newOp = op->clone(sim);
   newOp->onBeforeOperationBegin(sim);
   newOp->convertBegin(sim);
   sim->addOperation(newOp);
   sim->addTimePoint(newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()));
   newOp->onAfterOperationBegin(sim);
   return true;
}

bool CheckOperations::operator()(RDOIE *ie)
{
   if(sim->getCurrentTime() >= ie->time)
   {
      ie->onBeforeIrregularEvent(sim);
      ie->convertEvent(sim);
      sim->addTimePoint(ie->time = (ie->getNextTimeInterval(sim) + sim->getCurrentTime()));
      ie->onAfterIrregularEvent(sim);
      return true;
   }

   return false;
}

bool CheckOperations::operator()(RDORule *ru)
{
   ru->onBeforeChoiceFrom(sim);
   if(ru->choiceFrom(sim))
   {
      ru->onBeforeRule(sim);
      ru->convertRule(sim);
      ru->onAfterRule(sim);
      return true;
   }
   return false;
}
*/
bool RDOSimulator::doOperation(bool onlyEndOfOperations)
{
	std::for_each(havePokaz.begin(), havePokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::checkPokaz), this));
	onAfterCheckPokaz();

   if(!onlyEndOfOperations)
   {
      // Check all operations
      if(std::find_if(haveBaseOperations.begin(), 
         haveBaseOperations.end(), CheckOperations(this)) != haveBaseOperations.end())
         return true;
		
/*
      // Check all decision points
      if(std::find_if(haveDecisionPoints.begin(), 
         haveDecisionPoints.end(), CheckOperations(this)) != haveDecisionPoints.end())
         return true;

      // Check all rules
      if(std::find_if(haveRules.begin(), 
         haveRules.end(), CheckOperations(this)) != haveRules.end())
         return true;

      // Check all operations
      if(std::find_if(haveOperations.begin(), 
         haveOperations.end(), CheckOperations(this)) != haveOperations.end())
         return true;

      // Check irregular events
      if(std::find_if(haveIrregularEvents.begin(), 
         haveIrregularEvents.end(), CheckOperations(this)) != haveIrregularEvents.end())
         return true;
*/
   }

   if(checkEndOfOperation())
      return true;

   return false;
}

void RDOSimulator::rdoDestroy()
{
   RDOSimulatorBase::rdoDestroy();

//   DeleteAllObjects(haveOperations);
//   DeleteAllObjects(haveIrregularEvents);
   DeleteAllObjects(operations);
//   DeleteAllObjects(haveDecisionPoints);
//   DeleteAllObjects(haveRules);
}
                      
void RDOSimulator::rdoInit()
{
   RDOSimulatorBase::rdoInit();
}

void RDOSimulator::preProcess()
{
   // Initialise all irregular events
/*
   for(std::list<RDOIE *>::iterator i = haveIrregularEvents.begin(); 
         i != haveIrregularEvents.end(); i++)
	{
      (*i)->onBeforeIrregularEvent(this);
      addTimePoint((*i)->time = ((*i)->getNextTimeInterval(this) + getCurrentTime()));
      (*i)->onAfterIrregularEvent(this);
   }
*/
   for(std::list<RDOBaseOperation *>::iterator i = haveBaseOperations.begin(); 
         i != haveBaseOperations.end(); i++)
   {
		RDOIE *currIE = dynamic_cast<RDOIE *>(*i);
		if(currIE) // definitely IE
		{
			currIE->onBeforeIrregularEvent(this);
			addTimePoint(currIE->time = (currIE->getNextTimeInterval(this) + getCurrentTime()));
			currIE->onAfterIrregularEvent(this);
		}	
   }

	std::for_each(havePokaz.begin(), havePokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::resetPokaz), this));
}

void RDOSimulator::postProcess()
{
	std::for_each(havePokaz.begin(), havePokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::calcStat), this));
}

void RDOSimulator::addOperation(RDOOperation *op) 
{
   operations.push_back(op); 
}

bool RDOSimulator::checkEndOfOperation()
{
   for(std::list<RDOOperation *>::iterator i = operations.begin(); 
         i != operations.end(); i++)
   {
      RDOOperation *op = (*i);
      if(getCurrentTime() >= op->time)
      {
         op->onBeforeOperationEnd(this);
         op->convertEnd(this);
         op->onAfterOperationEnd(this);
         i = operations.erase(i);
			i--;
         delete op;
         return true;
      }
   }
   
   return false;
}

RDODecisionPoint::~RDODecisionPoint()
{                                      
   DeleteAllObjects(activities);
}

RDOActivity::~RDOActivity()
{
   delete rule;
}

TreeRoot *RDODecisionPoint::createTreeRoot(RDOSimulator *sim)
{
   return new TreeRoot(sim, this);
}

void RDODecisionPoint::addActivity(RDOActivity *act) 
{ 
   activities.push_back(act); 
}

RDOSimulator *RDOSimulator::createCopy()
{
   RDOSimulator *res = clone();
   res->setCurrentTime(getCurrentTime());
   return res;
}
