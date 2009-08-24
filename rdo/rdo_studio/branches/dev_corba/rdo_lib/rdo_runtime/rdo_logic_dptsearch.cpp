#include "pch.h"
#include "rdo_logic_dptsearch.h"
#include "searchtree.h"
#include "rdo_rule.h"
#include "rdo_runtime.h"

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

IBaseOperation::BOResult RDODPTSearch::onDoOperation( RDOSimulator* sim )
{
	// Ќачало поиска: вывели трасировку, обновили статистику
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return onContinue( sim );
}

IBaseOperation::BOResult RDODPTSearch::onContinue( RDOSimulator* sim )
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
//		TRACE( "решение... \n" );
		for ( TreeNode* i = treeRoot->m_targetNode; i->m_parent; i = i->m_parent ) {
#ifdef _DEBUG
			static_cast<RDORuntime*>(i->m_sim)->showResources(i->m_number);
#endif
			bestPath.push_front(i);
		}
//		TRACE( "решение... done\n" );
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
	return success ? IBaseOperation::BOR_done : IBaseOperation::BOR_cant_run;
}

void RDODPTSearch::addActivity(LPIDPTSearchActivity activity)
{
	m_activityList.push_back(activity); 
}

} // namespace rdoRuntime
