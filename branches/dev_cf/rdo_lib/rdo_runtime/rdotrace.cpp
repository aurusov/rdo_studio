#include "pch.h"
#include "rdotrace.h"
#include "searchtrace.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

#ifdef RDOSIM_COMPATIBLE
std::string doubleToString( double value )
{
	std::ostringstream _str;
	_str << value;
	std::string::size_type pos = _str.str().find( "e" );
	if ( pos != std::string::npos ) {
		std::string __str = _str.str();
		__str.erase( pos + 2, 1 );
		return __str.c_str();
	} else {
		return _str.str().c_str();
	}
}
#else
double doubleToString( double value )
{
	return value;
}
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTrace - Формирует строки трассировки
// ----------------------------------------------------------------------------
void RDOTrace::writeSearchBegin( double currentTime, std::string decisionPointId )
{
	if ( !canTrace() ) return;

	getOStream() << "SB " << currentTime << " " << decisionPointId.c_str() << std::endl << getEOL();
}

void RDOTrace::writeSearchDecisionHeader()
{
	if ( !canTrace() ) return;

	getOStream() << "SD" << std::endl << getEOL();
}

void RDOTrace::writeSearchDecision(RDOSimulator *sim, TreeNode *node) 
{
	if ( !canTrace() ) return;

	RDOSimulatorTrace* simTr  = (RDOSimulatorTrace*)sim;
	RDOActivityTrace*  actTr  = (RDOActivityTrace*)node->m_activity;
	RDORule*           ruleTr = (RDORule*)actTr->rule();

	getOStream() << node->m_number
	             << " " << actTr->traceId()
	             << " " << ruleTr->tracePatternId()
	             << " " << ruleTr->traceResourcesListNumbers( simTr )
	             << std::endl << getEOL();
}

void RDOTrace::writeString(std::string str)
{
	if ( !canTrace() ) return;
	
	getOStream() << str << std::endl << getEOL();
}

void RDOTrace::writeSearchOpenNode( int nodeCount, int parentCount, double pathCost, double restCost )
{
	if ( !canTrace() ) return;

	getOStream() << "SO " << nodeCount
	             << " " << doubleToString(parentCount)
	             << " " << doubleToString(pathCost)
	             << " " << doubleToString(restCost) << std::endl << getEOL();
}

void RDOTrace::writeSearchNodeInfo(char sign, TreeNodeTrace *node)
{
	if ( !canTrace() ) return;

	RDODPTSearchTrace* dpTrace = static_cast<RDODPTSearchTrace*>(node->m_root->m_dp);
	if ( dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_tops || dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all ) {
		RDOActivityTrace*  actTr = static_cast<RDOActivityTrace*>(node->m_currAct);
		RDORule*           rule  = static_cast<RDORule*>(actTr->rule());
		RDOSimulatorTrace* sim   = static_cast<RDOSimulatorTrace*>(node->m_sim);

		getOStream().precision(4);
		getOStream() << "ST" << sign
		             << " " << ((sign != 'D') ? node->m_number : node->m_root->getNodesCound())
		             << " " << ((sign != 'D') ? node->m_parent->m_number : node->m_number)
		             << " " << ((sign != 'D') ? doubleToString(node->m_costPath) : doubleToString(node->m_newCostPath) )
		             << " " << ((sign != 'D') ? doubleToString(node->m_costRest) : doubleToString(node->m_newCostRest) )
		             << " " << actTr->traceId()
		             << " " << rule->tracePatternId()
		             << " " << ((sign != 'D') ? doubleToString(node->m_costRule) : doubleToString(node->m_newCostRule) )
		             << " " << rule->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();

		RDODPTSearchTrace* dpTrace = static_cast<RDODPTSearchTrace*>(node->m_root->m_dp);
		if ( dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all ) {
			getOStream() << rule->traceResourcesList( 'S', sim ) << getEOL();
		}
	}
}

void RDOTrace::writeSearchResult( char letter, RDOSimulatorTrace* simTr, TreeRoot* treeRoot )
{
	if ( !canTrace() ) return;

	SYSTEMTIME systime_current;
	::GetSystemTime( &systime_current );
	unsigned int msec_current = RDOSimulatorBase::getMSec( systime_current );
	unsigned int msec_begin   = RDOSimulatorBase::getMSec( treeRoot->m_systime_begin );
	double sec_delay = 0;
	if ( systime_current.wYear == treeRoot->m_systime_begin.wYear && systime_current.wMonth == treeRoot->m_systime_begin.wMonth ) {
		sec_delay = static_cast<double>(msec_current - msec_begin) / 1000 + (systime_current.wDay - treeRoot->m_systime_begin.wDay) * 24 * 60 * 60;
	}
	static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_times.push_back( sec_delay );

	getOStream() << "SE" << letter
	             << " " << simTr->getCurrentTime()
	             << " " << sec_delay            // realTime
	             << " " << treeRoot->m_sizeof_dpt // memUsed
	             << " " << (letter == 'S' ? treeRoot->m_targetNode->m_costPath : 0)
	             << " " << treeRoot->m_expandedNodesCount
	             << " " << treeRoot->m_nodesInGraphCount
	             << " " << treeRoot->getNodesCound()
	             << " " << treeRoot->m_fullNodesCount 
	             << std::endl << getEOL();
	if ( letter == 'S' ) {
		getOStream() << "ES"
		             << " " << simTr->getCurrentTime()
		             << " 4"
		             << std::endl << getEOL();
		static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_cost.push_back( treeRoot->m_targetNode->m_costPath );
		static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_nodes.push_back( treeRoot->getNodesCound() );
		static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_nodes_expended.push_back( treeRoot->m_expandedNodesCount );
		static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_nodes_full.push_back( treeRoot->m_fullNodesCount );
		static_cast<RDODPTSearchTrace*>(treeRoot->m_dp)->calc_nodes_in_graph.push_back( treeRoot->m_nodesInGraphCount );
	}
}

void RDOTrace::writePermanentResources( rdoRuntime::RDOSimulatorTrace* sim, const std::list< RDOResource* >& res_perm )
{
	if ( !canTrace() ) return;

	getOStream() << traceResourcesList('\0', sim, res_perm) << getEOL();
}

std::string RDOTrace::traceResourcesList( char prefix, RDOSimulatorTrace* sim, const std::list< RDOResource* >& rel_res_list )
{
	std::string res;
	for ( std::list< RDOResource* >::const_iterator i = rel_res_list.begin(); i != rel_res_list.end(); i++ ) {
		if ( *i ) {
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

void RDOTrace::writeIrregularEvent(RDOIrregEvent *ie, RDOSimulatorTrace *sim)
{
	if ( !canTrace() ) return;

#ifdef RDOSIM_COMPATIBLE
	getOStream() << ie->traceResourcesList( '\0', sim ) << getEOL();
#endif

	if ( ie->traceable() ) {
		getOStream() << "EI " << sim->getCurrentTime()
		             << " "   << ie->traceId() 
		             << " "   << ie->tracePatternId() 
		             << " "   << ie->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();
	}

#ifndef RDOSIM_COMPATIBLE
	getOStream() << ie->traceResourcesList( '\0', sim ) << getEOL();
#endif
}

void RDOTrace::writeRule( RDORule* rule, RDOSimulatorTrace* sim )
{
	if ( !canTrace() ) return;

	if ( rule->traceable() ) {
		int operId = sim->getFreeOperationId();
		getOStream() << "ER " << sim->getCurrentTime()
		             << " "   << operId
		             << " "   << rule->traceId() 
		             << " "   << rule->tracePatternId()
		             << " "   << rule->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL();
		sim->freeOperationId(operId);
	}
	getOStream() << rule->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationBegin( RDOOperation* op, RDOSimulatorTrace* sim )
{
	if ( !canTrace() ) return;

	if ( op->traceable() ) {
		getOStream() << "EB " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL(); 
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationEnd( RDOOperation* op, RDOSimulatorTrace* sim )
{
	if ( !canTrace() ) return;

	if ( op->traceable() ) {
		getOStream() << "EF " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL();
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeTraceBegin(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

void RDOTrace::writeTraceEnd(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 2" << std::endl << getEOL();
}

void RDOTrace::writeStatus( RDOSimulatorTrace* sim, char* status )
{
	if ( isNull() ) return;

	// Статус
	getOStream() << "$Status = " << status << " " << sim->getCurrentTime() << std::endl << getEOL();

	// Статистика по поиску на графе
	RDOLogic::CIterator it = sim->m_logics.begin();
	while ( it != sim->m_logics.end() ) {
		RDODPTSearchTrace* dp = dynamic_cast<RDODPTSearchTrace*>(*it);
		if ( dp ) {
			// Информация о точке
			getOStream() << std::endl << getEOL();
			getOStream() << "DPS_C"
			             << "  " << dp->getTraceID()
			             << "  " << dp->calc_cnt
			             << "  " << dp->calc_res_found_cnt
			             << std::endl << getEOL();
			if ( dp->calc_cnt ) {
				// Время поиска
				double d_min = 0;
				double d_max = 0;
				double d_med = 0;
				dp->getStats( dp->calc_times, d_min, d_max, d_med );
				getOStream() << rdo::format( "DPS_TM %0.3f  %0.3f  %0.3f", d_med, d_min, d_max ) << std::endl << getEOL();

				// Используемая память
				unsigned int ui_min = 0;
				unsigned int ui_max = 0;
				dp->getStats( dp->calc_mems, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_ME %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Стоимость решения
				dp->getStats( dp->calc_cost, d_min, d_max, d_med );
				getOStream() << "DPS_CO"
				             << " "  << d_med
				             << "  " << d_min
				             << "  " << d_max
				             << std::endl << getEOL();

				// Количество раскрытых вершин
				dp->getStats( dp->calc_nodes_expended, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TO %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Количество вершин в графе
				dp->getStats( dp->calc_nodes_in_graph, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TT %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Количество включавшихся в граф вершин (вершины, соответствующие одному и тому же состоянию системы, могут включаться в граф неоднократно, если порождается вершина с меньшей стоимостью пути)
				dp->getStats( dp->calc_nodes, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TI %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Общее количество порожденных вершин-преемников
				dp->getStats( dp->calc_nodes_full, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TG %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();
			}
		}
		it++;
	}

	// Используемая память
	getOStream() << std::endl << getEOL();
	getOStream() << "DPS_MM " << sim->memory_get() << std::endl << getEOL();
}

void RDOTrace::writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok)
{
	if ( !canTrace() ) return;

	getOStream() << "V  "  << sim->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

// ----------------------------------------------------------------------------
// ---------- RDOPokazTrace
// ----------------------------------------------------------------------------
RDOPokazTrace::RDOPokazTrace( RDORuntime* runtime, bool trace ):
	RDOPokaz( runtime ),
	RDOTraceableObject( trace ),
	RDORuntimeContainer( runtime ),
	m_wasChanged( true )
{
}

bool RDOPokazTrace::tracePokaz()
{
	if ( !traceable() || !m_wasChanged ) {
		return false;
	}
	getRuntime()->getTracer()->writePokaz( getRuntime(), this );
	m_wasChanged = false;
	return true;
}

} // namespace rdoRuntime;
