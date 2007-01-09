#include "pch.h"
#include "rdoprocess.h"

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

bool RDOOperation::checkOperation( RDOSimulator* sim )
{
	// Если операция может начаться, то создать её клон и поместить его в список
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		RDOOperation* newOp = clone( sim );
		newOp->onBeforeOperationBegin( sim );
		newOp->convertBegin( sim );
		sim->addTimePoint( newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()) );
		newOp->onAfterOperationBegin( sim );
		newOp->convert_end = true;
		operation_clone.push_back( newOp );
		return true;
	} else {
		// Операция начаться не может, проверим события конца всех её клонов
		double time_now = sim->getCurrentTime();
		std::list<RDOOperation *>::iterator it = operation_clone.begin();
		while ( it != operation_clone.end() ) {
			// Выполняем событие конца
			if ( time_now >= (*it)->time ) {
				(*it)->onBeforeOperationEnd( sim );
				(*it)->convertEnd( sim );
				(*it)->onAfterOperationEnd( sim );
				delete *it;
				operation_clone.erase( it );
				return true;
			}
			it++;
		}
	}
	return false;
}

bool RDOIE::checkOperation(RDOSimulator *sim)
{
   if(sim->getCurrentTime() >= time)
   {
      onBeforeIrregularEvent(sim);
      convertEvent(sim);
      sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()) );
      onAfterIrregularEvent(sim);
      return true;
   }

   return false;
}

bool RDORule::checkOperation( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		onBeforeRule( sim );
		convertRule( sim );
		onAfterRule( sim );
		return true;
	}
	return false;
}

bool RDOSimulator::doOperation()
{
	onCheckPokaz();
	onAfterCheckPokaz();

	// Проверить все возможные действия и вызвать первое, которое может буть вызвано
	return std::find_if( haveBaseOperations.begin(), haveBaseOperations.end(), CheckOperations(this) ) != haveBaseOperations.end();
}

void RDOSimulator::rdoDestroy()
{
	RDOSimulatorBase::rdoDestroy();
}

void RDOSimulator::rdoInit()
{
   RDOSimulatorBase::rdoInit();
}

void RDOSimulator::preProcess()
{
	for ( std::vector< RDOBaseOperation* >::const_iterator i = haveBaseOperations.begin(); i != haveBaseOperations.end(); i++ ) {
		RDOIE* currIE = dynamic_cast<RDOIE*>(*i);
		if ( currIE ) {
			currIE->onBeforeIrregularEvent( this );
			addTimePoint( currIE->time = (currIE->getNextTimeInterval( this ) + getCurrentTime()) );
		}
		rdoRuntime::RDOPROCProcess* process = dynamic_cast<rdoRuntime::RDOPROCProcess*>(*i);
		if ( process ) {
			process->preProcess( this );
		}	
	}
	onResetPokaz();
}

RDODecisionPoint::~RDODecisionPoint()
{                                      
	DeleteAllObjects( activities );
}

RDOActivity::~RDOActivity()
{
   delete rule;
}

TreeRoot *RDODecisionPoint::createTreeRoot(RDOSimulator *sim)
{
	return new TreeRoot( sim, this );
}

void RDODecisionPoint::addActivity(RDOActivity *act) 
{ 
   activities.push_back(act); 
}

RDOSimulator *RDOSimulator::createCopy()
{
	RDOSimulator* res = clone();
	res->setCurrentTime(getCurrentTime());
	return res;
}
