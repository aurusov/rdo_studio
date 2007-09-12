#include "pch.h"
#include "searchtrace.h"
#include "rdotrace.h"
#include "simtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

RDOActivityTrace::RDOActivityTrace( RDOSimulatorTrace* i_sim, RDORule* r, bool vA ):
	RDOActivity( r, vA ),
	RDOTraceableObject( i_sim )
{
	// Т.е. тут можно было написать id = r->id, тольуо r динамически
	// приведен к типу RDORule, в котром нет id.
	// Присвоение и увеличение i_sim->activityCounter на один было сделано
	// в конструкторе r. Текущее значение i_sim->activityCounter на 1 больше.
	id = i_sim->activityCounter - 1;
}

void RDODecisionPointTrace::onSearchBegin( RDOSimulator* sim )
{
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	if ( traceFlag != DPT_no_trace ) {
		simTr->getTracer()->writeSearchBegin(simTr->getCurrentTime(), traceId());
	}
	if ( traceFlag == DPT_trace_tops || traceFlag == DPT_trace_all ) {
		simTr->getTracer()->writeString( "STN 1 0 0 0 -1 -1 0 0" );
	}
	calc_cnt++;
}

void RDODecisionPointTrace::onSearchDecisionHeader( RDOSimulator* sim )
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
		simTr->getTracer()->writeSearchDecisionHeader();
	}
}

void RDODecisionPointTrace::onSearchDecision( RDOSimulator* sim, TreeNode* node )
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
		simTr->getTracer()->writeSearchDecision( sim, node );
	}
}

void RDODecisionPointTrace::onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )
{
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	if ( traceFlag != DPT_no_trace ) {
		simTr->getTracer()->writeSearchResult( 'S', simTr, treeRoot );
	}
	calc_res_found_cnt++;
	calc_mems.push_back( treeRoot->sizeof_dpt );
	simTr->memory_insert( treeRoot->sizeof_dpt );
}

void RDODecisionPointTrace::onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot)
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
		simTr->getTracer()->writeSearchResult( 'N', simTr, treeRoot );
	}
}

void TreeNodeTrace::onSearchOpenNode( RDOSimulator* sim )
{
	RDODecisionPointTrace* dpTrace = (RDODecisionPointTrace *)root->dp;
	if ( dpTrace->traceFlag == RDODecisionPointTrace::DPT_trace_tops || dpTrace->traceFlag == RDODecisionPointTrace::DPT_trace_all ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace *)sim;
		simTr->getTracer()->writeSearchOpenNode( count,
			(parent ? parent->count : 0 ),
			costPath,
			costRest );
	}
}

void TreeNodeTrace::onSearchNodeInfoDeleted( RDOSimulator* sim )
{
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace *)sim;
	simTr->getTracer()->writeSearchNodeInfo( 'D', this );
}

void TreeNodeTrace::onSearchNodeInfoReplaced( RDOSimulator* sim )
{
   RDOSimulatorTrace* simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeSearchNodeInfo( 'R', this ); // must be 'R'
}

void TreeNodeTrace::onSearchNodeInfoNew( RDOSimulator* sim )
{
   RDOSimulatorTrace* simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeSearchNodeInfo( 'N', this );
}


TreeNode* TreeNodeTrace::createChildTreeNode()
{
	root->sizeof_dpt += sizeof( TreeNode );
	return new TreeNodeTrace( childSim, this, root, currAct, costPath, root->nodeCount++ );
}

void TreeRootTrace::createRootTreeNode( RDOSimulator* sim )
{
	rootNode = new TreeNodeTrace( sim, NULL, this, NULL, 0, nodeCount++ );
	rootNode->costRule = 0;
	rootNode->costPath = 0;
	rootNode->costRest = 0;
	allLeafs.push_back( rootNode );
	sizeof_dpt += sizeof( TreeNodeTrace ) + sizeof( void* );
}

TreeRoot* RDODecisionPointTrace::createTreeRoot( RDOSimulator* sim )
{
	TreeRootTrace* root = new TreeRootTrace( sim, this );
	root->sizeof_dpt = sizeof(TreeRootTrace) + sim->sizeof_sim;
	return root;
}

void RDODecisionPointTrace::getStats( std::list< double >& list, double& min, double& max, double& med ) const
{
	double sum = 0;
	int    cnt = 0;
	std::list< double >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		sum += *it;
		if ( !cnt ) {
			min = *it;
			max = *it;
		}
		cnt++;
		if ( min > *it ) {
			min = *it;
		}
		if ( max < *it ) {
			max = *it;
		}
		it++;
	}
	if ( cnt ) {
		med = sum / cnt;
	}
}

void RDODecisionPointTrace::getStats( std::list< unsigned int >& list, unsigned int& min, unsigned int& max, double& med ) const
{
	unsigned int sum = 0;
	int          cnt = 0;
	std::list< unsigned int >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		sum += *it;
		if ( !cnt ) {
			min = *it;
			max = *it;
		}
		cnt++;
		if ( min > *it ) {
			min = *it;
		}
		if ( max < *it ) {
			max = *it;
		}
		it++;
	}
	if ( cnt ) {
		med = (double)sum / cnt;
	}
}

} // namespace rdoRuntime;
