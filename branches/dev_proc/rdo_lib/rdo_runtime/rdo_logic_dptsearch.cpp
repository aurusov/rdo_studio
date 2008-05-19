#include "pch.h"
#include "rdo_logic_dptsearch.h"
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
RDODPTSearch::RDODPTSearch( RDOSimulator* sim ):
	RDOLogic( sim ),
	treeRoot( NULL )
{
}

RDODPTSearch::~RDODPTSearch()
{
}

RDOBaseOperation::BOResult RDODPTSearch::onDoOperation( RDOSimulator* sim )
{
	// Ќачало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return onContinue( sim );
}

RDOBaseOperation::BOResult RDODPTSearch::onContinue( RDOSimulator* sim )
{
	DWORD time_begin = ::GetTickCount();
	while ( true ) {
		// ¬озмем дл€ раскрыти€ первую вершину из списка OPEN
		TreeNode* curr = *(treeRoot->m_OPEN.begin());
		curr->ExpandChildren();
		if ( treeRoot->m_OPEN.empty() || treeRoot->m_targetNode ) break;

		DWORD time_current = ::GetTickCount();
		if ( time_current - time_begin > 1000 / 40 ) {
			return BOR_must_continue;
		}
	}

	bool success = treeRoot->m_targetNode ? true : false;
	if ( success ) {
		// Ќашли решение, собрали путь
		std::list< TreeNode* > bestPath;
		TRACE( "решение... \n" );
		for ( TreeNode* i = treeRoot->m_targetNode; i->m_parent; i = i->m_parent ) {
#ifdef _DEBUG
			static_cast<RDORuntime*>(i->m_sim)->showResources(i->m_number);
#endif
			bestPath.push_front(i);
		}
		TRACE( "решение... done\n" );
		// ќтработали предварительные действи€: вывели трассировку
		onSearchDecisionHeader( treeRoot->m_theRealSimulator );
		// ќтработали рулы
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->m_activity->rule()->onBeforeChoiceFrom( treeRoot->m_theRealSimulator );
			node->m_activity->rule()->choiceFrom( static_cast<RDORuntime*>(treeRoot->m_theRealSimulator) );
			node->m_activity->rule()->onBeforeRule( treeRoot->m_theRealSimulator );
			node->m_activity->rule()->convertRule( static_cast<RDORuntime*>(treeRoot->m_theRealSimulator) );
			node->m_activity->rule()->onAfterRule( treeRoot->m_theRealSimulator, true );
			// ќтработали каждую вершину: вывели трассировку
			onSearchDecision( treeRoot->m_theRealSimulator, node );
		}
		// ќтработали завершающие действи€: вывели трассировку, обновили статистику по поиску
		onSearchResultSuccess( treeRoot->m_theRealSimulator, treeRoot );
	} else {
		// Ќеудачное завершение поиска: вывели статистику
		onSearchResultNotFound( treeRoot->m_theRealSimulator, treeRoot );
	}
	delete treeRoot->m_rootNode;
	delete treeRoot;
	treeRoot = NULL;
	return success ? RDOBaseOperation::BOR_done : RDOBaseOperation::BOR_cant_run;
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
