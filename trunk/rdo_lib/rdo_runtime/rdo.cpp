#include "pch.h"
#include "rdo.h"
#include "rdoprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- CheckOperations - фуктрорал проверки на запуск RDOBaseOperation
// ----------------------------------------------------------------------------
bool CheckOperations::operator()( RDOBaseOperation* opr )
{
	return opr->checkOperation( sim ) != RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOIE - irregular_event
// ----------------------------------------------------------------------------
void RDOIE::init( RDOSimulator* sim )
{
	onBeforeIrregularEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
}

RDOBaseOperation::BOResult RDOIE::checkOperation(RDOSimulator *sim)
{
	return RDOBaseOperation::BOR_cant_run;
}

void RDOIE::makePlaned( RDOSimulator* sim, void* param )
{
	onBeforeIrregularEvent( sim );
	convertEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
	onAfterIrregularEvent( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDORule - rule
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDORule::checkOperation( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		onBeforeRule( sim );
		convertRule( sim );
		onAfterRule( sim );
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOOperation - operation
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOOperation::checkOperation( RDOSimulator* sim )
{
	// Если операция может начаться, то создать её клон и поместить его в список
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		RDOOperation* newOp = clone( sim );
		newOp->reparent( &clones );
		newOp->onBeforeOperationBegin( sim );
		newOp->convertBegin( sim );
		sim->addTimePoint( newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()), this, newOp );
		newOp->onAfterOperationBegin( sim );
		newOp->convert_end = true;
		return RDOBaseOperation::BOR_planned_and_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

void RDOOperation::makePlaned( RDOSimulator* sim, void* param )
{
	// Выполняем событие конца операции-клона
	RDOOperation* opr = static_cast<RDOOperation*>(param);
	opr->onBeforeOperationEnd( sim );
	opr->convertEnd( sim );
	opr->onAfterOperationEnd( sim );
	delete opr;
}

// ----------------------------------------------------------------------------
// ---------- RDODecisionPoint - DPT
// ----------------------------------------------------------------------------
RDODecisionPoint::~RDODecisionPoint()
{
//qq	DeleteAllObjects( activities );
}

RDOBaseOperation::BOResult RDODecisionPoint::checkOperation( RDOSimulator* sim )
{
	if ( Condition(sim) ) {
		if ( RunSearchInTree(sim) ) {
			return RDOBaseOperation::BOR_can_run;
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

bool RDODecisionPoint::RunSearchInTree( RDOSimulator* sim )
{
	// Начало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	TreeRoot* treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	for (;;) {
		// Возмем для раскрытия первую вершину из списка OPEN
		TreeNode* curr = *(treeRoot->allLeafs.begin());
		curr->ExpandChildren();
		if ( treeRoot->allLeafs.empty() || treeRoot->targetNode ) break;
	}

	bool success = treeRoot->targetNode ? true : false;
	if ( success ) {
		// Нашли решение, собрали путь
		std::list< TreeNode* > bestPath;
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
			bestPath.push_front(i);
		}
		// Отработали предварительные действия: вывели трассировку
		onSearchDecisionHeader( treeRoot->theRealSimulator );
		// Отработали рулы
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->activity->rule->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->choiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule->convertRule( treeRoot->theRealSimulator );
			node->activity->rule->onAfterRule( treeRoot->theRealSimulator, true );
			// Отработали каждую вершину: вывели трассировку
			onSearchDecision( treeRoot->theRealSimulator, node );
		}
		// Отработали завершающие действия: вывели трассировку, обновили статистику по поиску
		onSearchResultSuccess( treeRoot->theRealSimulator, treeRoot );
	} else {
		// Неудачное завершение поиска: вывели статистику
		onSearchResultNotFound( treeRoot->theRealSimulator, treeRoot );
	}
	delete treeRoot->rootNode;
	delete treeRoot;
	return success;
}

void RDODecisionPoint::addActivity( RDOActivity* act )
{
	// Удалять из активности из activities не надо, т.к. это делает объект-родитель
	act->reparent( this );
	activities.push_back( act ); 
}

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool found_planed = false;
	// Отработаем все запланированные на данный момент события
	if ( !check_operation && !timePointList.empty() ) {
		check_operation = true;
		double newTime = timePointList.begin()->first;
		if ( getCurrentTime() >= newTime ) {
			std::list< BOPlanned >* list = timePointList.begin()->second;
			if ( list && !list->empty() ) {
#ifdef RDOSIM_COMPATIBLE
				// Дисциплина списка текущих событий LIFO
				RDOBaseOperation* opr   = list->back().opr;
				void*             param = list->back().param;
				list->pop_back();
#else
				// Дисциплина списка текущих событий FIFO
				RDOBaseOperation* opr   = list->front().opr;
				void*             param = list->front().param;
				list->pop_front();
#endif
				if ( list->empty() ) {
					delete timePointList.begin()->second;
					timePointList.erase( timePointList.begin() );
				}
				opr->makePlaned( this, param );
				found_planed = true;
			}
		}
	}
	bool res = found_planed;
	if ( !found_planed ) {
		// Не нашли запланированное событие
		// Проверить все возможные события и действия, вызвать первое, которое может буть вызвано
		res = std::find_if( haveBaseOperations.begin(), haveBaseOperations.end(), CheckOperations( this ) ) != haveBaseOperations.end();
		if ( !res ) check_operation = false;
	}
	onCheckPokaz();
	onAfterCheckPokaz();
	return res;
}

void RDOSimulator::preProcess()
{
	for ( std::vector< RDOBaseOperation* >::const_iterator it = haveBaseOperations.begin(); it != haveBaseOperations.end(); it++ ) {
		(*it)->init( this );
	}
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}

} // namespace rdoRuntime
