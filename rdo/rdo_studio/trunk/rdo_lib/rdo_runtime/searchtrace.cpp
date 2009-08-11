#include "pch.h"
#include "searchtrace.h"
#include "rdotrace.h"
#include "simtrace.h"
#include "rdo_rule.h"

namespace rdoRuntime
{

void RDODPTSearchTrace::onSearchBegin( RDOSimulator* sim )
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

void RDODPTSearchTrace::onSearchDecisionHeader( RDOSimulator* sim )
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
		simTr->getTracer()->writeSearchDecisionHeader();
	}
}

void RDODPTSearchTrace::onSearchDecision( RDOSimulator* sim, TreeNode* node )
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
		simTr->getTracer()->writeSearchDecision( sim, node );
	}
}

void RDODPTSearchTrace::onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )
{
	RDOSimulatorTrace* simTr = (RDOSimulatorTrace*)sim;
	if ( traceFlag != DPT_no_trace ) {
		simTr->getTracer()->writeSearchResult( 'S', simTr, treeRoot );
	}
	calc_res_found_cnt++;
	calc_mems.push_back( treeRoot->m_sizeof_dpt );
	simTr->memory_insert( treeRoot->m_sizeof_dpt );
}

void RDODPTSearchTrace::onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot)
{
	if ( traceFlag != DPT_no_trace ) {
		RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
		simTr->getTracer()->writeSearchResult( 'N', simTr, treeRoot );
	}
}

void TreeNodeTrace::onSearchOpenNode( RDOSimulator* sim )
{
	RDODPTSearchTrace* dpTrace = (RDODPTSearchTrace *)m_root->m_dp;
	if ( dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_tops || dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all ) {
		RDOSimulatorTrace* simTr = (RDOSimulatorTrace *)sim;
		simTr->getTracer()->writeSearchOpenNode( m_number,
			(m_parent ? m_parent->m_number : 0 ),
			m_costPath,
			m_costRest );
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
	m_root->m_sizeof_dpt += sizeof( TreeNode );
	return new TreeNodeTrace( m_childSim, this, m_root, m_currAct, m_costPath, m_root->getNewNodeNumber() );
}

void TreeRootTrace::createRootTreeNode( RDOSimulator* sim )
{
	m_rootNode = new TreeNodeTrace( sim, NULL, this, NULL, 0, getNewNodeNumber() );
	m_rootNode->m_costRule = 0;
	m_rootNode->m_costPath = 0;
	m_rootNode->m_costRest = 0;
	m_OPEN.push_back( m_rootNode );
	m_sizeof_dpt += sizeof( TreeNodeTrace ) + sizeof( void* );
}

TreeRoot* RDODPTSearchTrace::createTreeRoot( RDOSimulator* sim )
{
	TreeRootTrace* root = new TreeRootTrace( sim, this );
	root->m_sizeof_dpt = sizeof(TreeRootTrace) + sim->getSizeofSim();
	return root;
}

ruint RDODPTSearchTrace::getCalcCnt() const
{
	return calc_cnt;
}

ruint RDODPTSearchTrace::getCalcResFoundCnt() const
{
	return calc_res_found_cnt;
}

template <typename T>
void __getStats(CREF(std::list<T>) list, REF(T) min, REF(T) max, REF(double) med)
{
	T      sum = 0;
	ruint  cnt = 0;
	std::list<T>::const_iterator it = list.begin();
	while (it != list.end())
	{
		sum += *it;
		if (!cnt)
		{
			min = *it;
			max = *it;
		}
		cnt++;
		if (min > *it)
			min = *it;
		if (max < *it)
			max = *it;
		it++;
	}
	if (cnt)
	{
		med = (double)sum / cnt;
	}
}

void RDODPTSearchTrace::getStatsDOUBLE(Type type, REF(double) min, REF(double) max, REF(double) med) const
{
	switch (type)
	{
		case IDPTSearchTraceStatistics::ST_TIMES: return __getStats<double>(calc_times, min, max, med);
		case IDPTSearchTraceStatistics::ST_COST : return __getStats<double>(calc_cost , min, max, med);
	}
	NEVER_REACH_HERE;
}

void RDODPTSearchTrace::getStatsRUINT(Type type, REF(ruint) min, REF(ruint) max, REF(double) med) const
{
	switch (type)
	{
		case IDPTSearchTraceStatistics::ST_MEMORY        : return __getStats<ruint>(calc_mems          , min, max, med);
		case IDPTSearchTraceStatistics::ST_NODES         : return __getStats<ruint>(calc_nodes_in_graph, min, max, med);
		case IDPTSearchTraceStatistics::ST_NODES_FULL    : return __getStats<ruint>(calc_nodes_full    , min, max, med);
		case IDPTSearchTraceStatistics::ST_NODES_EXPENDED: return __getStats<ruint>(calc_nodes_expended, min, max, med);
		case IDPTSearchTraceStatistics::ST_NODES_IN_GRAPH: return __getStats<ruint>(calc_mems          , min, max, med);
	}
	NEVER_REACH_HERE;
}

} // namespace rdoRuntime
