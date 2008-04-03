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
// ---------- RDOIE - irregular_event
// ----------------------------------------------------------------------------
void RDOIE::init( RDOSimulator* sim )
{
	onBeforeIrregularEvent( sim );
	sim->addTimePoint( m_time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
}

bool RDOIE::checkOperation(RDOSimulator *sim)
{
	return false;
}

RDOBaseOperation::BOResult RDOIE::doOperation( RDOSimulator* sim )
{
	return RDOBaseOperation::BOR_cant_run;
}

void RDOIE::makePlaned( RDOSimulator* sim, void* param )
{
	sim->inc_cnt_events();
	onBeforeIrregularEvent( sim );
	convertEvent( sim );
	sim->addTimePoint( m_time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
	onAfterIrregularEvent( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDORule - rule
// ----------------------------------------------------------------------------
bool RDORule::checkOperation( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom(sim);
}

RDOBaseOperation::BOResult RDORule::doOperation( RDOSimulator* sim )
{
	onBeforeRule( sim );
	convertRule( sim );
	onAfterRule( sim );
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOOperation - operation
// ----------------------------------------------------------------------------
bool RDOOperation::checkOperation( RDOSimulator* sim )
{
	// Если операция может начаться, то создать её клон и поместить его в список
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	return choiceFrom(sim);
}

RDOBaseOperation::BOResult RDOOperation::doOperation( RDOSimulator* sim )
{
	RDOOperation* newOp = clone( sim );
	newOp->reparent( &clones );
	newOp->onBeforeOperationBegin( sim );
	newOp->convertBegin( sim );
	sim->addTimePoint( newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()), this, newOp );
	newOp->onAfterOperationBegin( sim );
	newOp->convert_end = true;
	return RDOBaseOperation::BOR_planned_and_run;
}

void RDOOperation::makePlaned( RDOSimulator* sim, void* param )
{
	// Выполняем событие конца операции-клона
	sim->inc_cnt_events();
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

bool RDODecisionPoint::checkOperation( RDOSimulator* sim )
{
	return Condition(sim);
}

RDOBaseOperation::BOResult RDODecisionPoint::doOperation( RDOSimulator* sim )
{
	return RunSearchInTree( sim );
}

RDOBaseOperation::BOResult RDODecisionPoint::continueOperation( RDOSimulator* sim )
{
	DWORD time_begin = ::GetTickCount();
	while ( true ) {
		// Возмем для раскрытия первую вершину из списка OPEN
		TreeNode* curr = *(treeRoot->OPEN.begin());
		curr->ExpandChildren();
		if ( treeRoot->OPEN.empty() || treeRoot->targetNode ) break;

		DWORD time_current = ::GetTickCount();
		if ( time_current - time_begin > 1000 / 40 ) {
			return BOR_must_continue;
		}
	}

	bool success = treeRoot->targetNode ? true : false;
	if ( success ) {
		// Нашли решение, собрали путь
		std::list< TreeNode* > bestPath;
		TRACE( "решение... \n" );
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
#ifdef _DEBUG
			static_cast<RDORuntime*>(i->sim)->showResources(i->number);
#endif
			bestPath.push_front(i);
		}
		TRACE( "решение... done\n" );
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
	treeRoot = NULL;
	return success ? RDOBaseOperation::BOR_done : RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODecisionPoint::RunSearchInTree( RDOSimulator* sim )
{
	// Начало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return continueOperation( sim );
}

void RDODecisionPoint::addActivity( RDOActivity* act )
{
	// Удалять активности из activities не надо, т.к. это делает объект-родитель
	act->reparent( this );
	activities.push_back( act ); 
}

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// Есть действие, которое необходимо продолжить. Используется в DPT
		RDOBaseOperation::BOResult result = getMustContinueOpr()->continueOperation( this );
		if ( result != RDOBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != RDOBaseOperation::BOR_cant_run;
	} else {
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
		res = found_planed;
		if ( !found_planed ) {
			// Не нашли запланированное событие
			// Проверить все возможные события и действия, вызвать первое, которое может буть вызвано
			res = m_logics.checkOperation(this);
			if ( res )
			{
				res = m_logics.doOperation(this) != RDOBaseOperation::BOR_cant_run;
			}
			if ( !res ) check_operation = false;
		}
	}
	onCheckPokaz();
	onAfterCheckPokaz();
	return res;
}

void RDOSimulator::preProcess()
{
	m_logics.init( this );
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}

} // namespace rdoRuntime
