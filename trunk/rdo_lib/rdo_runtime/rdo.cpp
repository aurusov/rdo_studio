#include "pch.h"
#include "rdo.h"
#include "searchtree.h"
#include "rdo_rule.h"
#include "rdo_runtime.h"

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
		// ¬озмем дл€ раскрыти€ первую вершину из списка OPEN
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
		// Ќашли решение, собрали путь
		std::list< TreeNode* > bestPath;
		TRACE( "решение... \n" );
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
#ifdef _DEBUG
			static_cast<RDORuntime*>(i->sim)->showResources(i->number);
#endif
			bestPath.push_front(i);
		}
		TRACE( "решение... done\n" );
		// ќтработали предварительные действи€: вывели трассировку
		onSearchDecisionHeader( treeRoot->theRealSimulator );
		// ќтработали рулы
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->activity->rule()->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule()->choiceFrom( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule()->convertRule( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onAfterRule( treeRoot->theRealSimulator, true );
			// ќтработали каждую вершину: вывели трассировку
			onSearchDecision( treeRoot->theRealSimulator, node );
		}
		// ќтработали завершающие действи€: вывели трассировку, обновили статистику по поиску
		onSearchResultSuccess( treeRoot->theRealSimulator, treeRoot );
	} else {
		// Ќеудачное завершение поиска: вывели статистику
		onSearchResultNotFound( treeRoot->theRealSimulator, treeRoot );
	}
	delete treeRoot->rootNode;
	delete treeRoot;
	treeRoot = NULL;
	return success ? RDOBaseOperation::BOR_done : RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODPTSearch::RunSearchInTree( RDOSimulator* sim )
{
	// Ќачало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return onContinue( sim );
}

void RDODPTSearch::addActivity( Activity* act )
{
	// ”дал€ть активности из activities не надо, т.к. это делает объект-родитель
	act->reparent( this );
	activities.push_back( act ); 
}

RDODPTSearch::Activity::Activity( RDORule* rule, ValueTime valueTime ):
	RDORuntimeObject( rule ),
	m_rule( rule ),
	m_valueTime( valueTime )
{
}

} // namespace rdoRuntime
