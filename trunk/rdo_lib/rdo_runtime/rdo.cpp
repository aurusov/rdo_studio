#include "pch.h"
#include "rdo.h"
#include "rdoprocess.h"
#include "searchtree.h"
#include "rdo_rule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::~RDODPTSearch()
{
//qq	DeleteAllObjects( activities );
}

bool RDODPTSearch::onCheckCondition( RDOSimulator* sim )
{
	return Condition(sim);
}

RDOBaseOperation::BOResult RDODPTSearch::onDoOperation( RDOSimulator* sim )
{
	return RunSearchInTree( sim );
}

RDOBaseOperation::BOResult RDODPTSearch::onContinue( RDOSimulator* sim )
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
			node->activity->rule()->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule()->choiceFrom( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule()->convertRule( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onAfterRule( treeRoot->theRealSimulator, true );
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

RDOBaseOperation::BOResult RDODPTSearch::RunSearchInTree( RDOSimulator* sim )
{
	// Начало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return onContinue( sim );
}

void RDODPTSearch::addActivity( Activity* act )
{
	// Удалять активности из activities не надо, т.к. это делает объект-родитель
	act->reparent( this );
	activities.push_back( act ); 
}

RDODPTSearch::Activity::Activity( RDORule* rule, ValueTime valueTime ):
	RDORuntimeObject( rule ),
	m_rule( rule ),
	m_valueTime( valueTime )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// Есть действие, которое необходимо продолжить. Используется в DPT
		RDOBaseOperation::BOResult result = getMustContinueOpr()->onContinue( this );
		if ( result != RDOBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != RDOBaseOperation::BOR_cant_run;
	} else {
		bool found_planed = false;
		// Отработаем все запланированные на данный момент события
		if ( !check_operation && !m_timePoints.empty() ) {
			check_operation = true;
			double newTime = m_timePoints.begin()->first;
			if ( getCurrentTime() >= newTime ) {
				BOPlannedItem* list = m_timePoints.begin()->second;
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
						delete list;
//						delete m_timePoints.begin()->second;
						m_timePoints.erase( m_timePoints.begin() );
					}
					opr->onMakePlaned( this, param );
					found_planed = true;
				}
			}
		}
		res = found_planed;
		if ( !found_planed ) {
			// Не нашли запланированное событие
			// Проверить все возможные события и действия, вызвать первое, которое может буть вызвано
			res = m_logics.onCheckCondition(this);
			if ( res )
			{
				res = m_logics.onDoOperation(this) != RDOBaseOperation::BOR_cant_run;
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
	m_logics.onStart( this );
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}

} // namespace rdoRuntime
