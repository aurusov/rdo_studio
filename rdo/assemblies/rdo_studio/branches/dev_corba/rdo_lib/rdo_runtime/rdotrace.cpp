#include "pch.h"
#include "rdotrace.h"
#include "searchtrace.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

#ifdef RDOSIM_COMPATIBLE
tstring doubleToString(double value)
{
	std::ostringstream _str;
	_str << value;
	tstring::size_type pos = _str.str().find("e");
	if (pos != tstring::npos) {
		tstring __str = _str.str();
		__str.erase(pos + 2, 1);
		return __str.c_str();
	} else {
		return _str.str().c_str();
	}
}
#else
double doubleToString(double value)
{
	return value;
}
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTrace - Формирует строки трассировки
// ----------------------------------------------------------------------------
void RDOTrace::writeSearchBegin(double currentTime, tstring decisionPointId)
{
	if (!canTrace())
		return;

	getOStream() << "SB " << currentTime << " " << decisionPointId.c_str() << std::endl << getEOL();
}

void RDOTrace::writeSearchDecisionHeader()
{
	if (!canTrace())
		return;

	getOStream() << "SD" << std::endl << getEOL();
}

void RDOTrace::writeSearchDecision(PTR(RDOSimulator) sim, PTR(TreeNode) node)
{
	if (!canTrace())
		return;

	PTR(RDOSimulatorTrace)  simTrace             = static_cast<PTR(RDOSimulatorTrace)>(sim);
	LPIActivityTrace        activityTrace        = node->m_activity->rule();
	LPIActivityPatternTrace activityPatternTrace = node->m_activity->rule();
	LPITrace                trace                = node->m_activity;
	ASSERT(trace);
	ASSERT(activityTrace);
	ASSERT(activityPatternTrace);

	getOStream() << node->m_number
	             << " " << trace->traceId()
	             << " " << activityPatternTrace->tracePatternId()
	             << " " << activityTrace->traceResourcesListNumbers(simTrace, true)
	             << std::endl << getEOL();
}

void RDOTrace::writeString(tstring str)
{
	if (!canTrace())
		return;
	
	getOStream() << str << std::endl << getEOL();
}

void RDOTrace::writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost)
{
	if (!canTrace())
		return;

	getOStream() << "SO " << nodeCount
	             << " " << doubleToString(parentCount)
	             << " " << doubleToString(pathCost)
	             << " " << doubleToString(restCost) << std::endl << getEOL();
}

void RDOTrace::writeSearchNodeInfo(char sign, PTR(TreeNodeTrace) node)
{
	if (!canTrace())
		return;

	PTR(RDODPTSearchTrace) dpTrace = static_cast<PTR(RDODPTSearchTrace)>(node->m_root->m_dp);
	if (dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_tops || dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all)
	{
		PTR(RDOSimulatorTrace)  simTrace             = static_cast<PTR(RDOSimulatorTrace)>(node->m_sim);
		LPIActivityTrace        activityTrace        = node->m_currAct->rule();
		LPIActivityPatternTrace activityPatternTrace = node->m_currAct->rule();
		LPITrace                trace                = node->m_currAct;
		ASSERT(trace);
		ASSERT(activityTrace);
		ASSERT(activityPatternTrace);

		getOStream().precision(4);
		getOStream() << "ST" << sign
		             << " " << ((sign != 'D') ? node->m_number : node->m_root->getNodesCound())
		             << " " << ((sign != 'D') ? node->m_parent->m_number : node->m_number)
		             << " " << ((sign != 'D') ? doubleToString(node->m_costPath) : doubleToString(node->m_newCostPath))
		             << " " << ((sign != 'D') ? doubleToString(node->m_costRest) : doubleToString(node->m_newCostRest))
		             << " " << trace->traceId()
		             << " " << activityPatternTrace->tracePatternId()
		             << " " << ((sign != 'D') ? doubleToString(node->m_costRule) : doubleToString(node->m_newCostRule))
		             << " " << activityTrace->traceResourcesListNumbers(simTrace, true)
		             << std::endl << getEOL();

		PTR(RDODPTSearchTrace) dpTrace = static_cast<PTR(RDODPTSearchTrace)>(node->m_root->m_dp);
		if (dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all)
		{
			getOStream() << activityTrace->traceResourcesList('S', simTrace) << getEOL();
		}
	}
}

void RDOTrace::writeSearchResult(char letter, PTR(RDOSimulatorTrace) simTr, PTR(TreeRoot) treeRoot)
{
	if (!canTrace())
		return;

	SYSTEMTIME systime_current;
	::GetSystemTime(&systime_current);
	unsigned int msec_current = RDOSimulatorBase::getMSec(systime_current);
	unsigned int msec_begin   = RDOSimulatorBase::getMSec(treeRoot->m_systime_begin);
	double sec_delay = 0;
	if (systime_current.wYear == treeRoot->m_systime_begin.wYear && systime_current.wMonth == treeRoot->m_systime_begin.wMonth) {
		sec_delay = static_cast<double>(msec_current - msec_begin) / 1000 + (systime_current.wDay - treeRoot->m_systime_begin.wDay) * 24 * 60 * 60;
	}
	static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_times.push_back(sec_delay);

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
	if (letter == 'S') {
		getOStream() << "ES"
		             << " " << simTr->getCurrentTime()
		             << " 4"
		             << std::endl << getEOL();
		static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_cost.push_back(treeRoot->m_targetNode->m_costPath);
		static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_nodes.push_back(treeRoot->getNodesCound());
		static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_nodes_expended.push_back(treeRoot->m_expandedNodesCount);
		static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_nodes_full.push_back(treeRoot->m_fullNodesCount);
		static_cast<PTR(RDODPTSearchTrace)>(treeRoot->m_dp)->calc_nodes_in_graph.push_back(treeRoot->m_nodesInGraphCount);
	}
}

void RDOTrace::writePermanentResources(rdoRuntime::PTR(RDOSimulatorTrace) sim, const std::list< RDOResource* >& res_perm)
{
	if (!canTrace())
		return;

	getOStream() << traceResourcesList('\0', sim, res_perm) << getEOL();
}

tstring RDOTrace::traceResourcesList(char prefix, PTR(RDOSimulatorTrace) sim, const std::list< RDOResource* >& rel_res_list)
{
	tstring res;
	for (std::list< RDOResource* >::const_iterator i = rel_res_list.begin(); i != rel_res_list.end(); i++) {
		if (*i) {
			res += (*i)->traceResourceState(prefix, sim);
		}
	}
	return res;
}

void RDOTrace::writeIrregularEvent(CREF(LPIBaseOperation) opr, PTR(RDOSimulatorTrace) sim)
{
	if (!canTrace())
		return;

	LPIActivityTrace activityTrace = opr;
	ASSERT(activityTrace);

#ifdef RDOSIM_COMPATIBLE
	getOStream() << activityTrace->traceResourcesList('\0', sim) << getEOL();
#endif

	LPITrace trace = opr;
	ASSERT(trace);

	if (trace->traceable())
	{
		LPIActivityPatternTrace activityPatternTrace = opr;
		ASSERT(activityPatternTrace);

		getOStream() << "EI " << sim->getCurrentTime()
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId() 
		             << " "   << activityTrace->traceResourcesListNumbers(sim, true)
		             << std::endl << getEOL();
	}

#ifndef RDOSIM_COMPATIBLE
	getOStream() << activityTrace->traceResourcesList('\0', sim) << getEOL();
#endif
}

void RDOTrace::writeRule(CREF(LPIBaseOperation) opr, PTR(RDOSimulatorTrace) sim)
{
	if (!canTrace())
		return;

	LPITrace         trace         = opr;
	LPIActivityTrace activityTrace = opr;
	ASSERT(trace);
	ASSERT(activityTrace);

	if (trace->traceable())
	{
		LPIActivityPatternTrace activityPatternTrace = opr;
		ASSERT(activityPatternTrace);
		int operId = sim->getFreeOperationId();
		getOStream() << "ER " << sim->getCurrentTime()
		             << " "   << operId
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId()
		             << " "   << activityTrace->traceResourcesListNumbers(sim, false)
		             << std::endl << getEOL();
		sim->freeOperationId(operId);
	}
	getOStream() << activityTrace->traceResourcesList('\0', sim) << getEOL();
}

void RDOTrace::writeAfterOperationBegin(CREF(LPIBaseOperation) opr, PTR(RDOSimulatorTrace) sim)
{
	if (!canTrace())
		return;

	LPITrace         trace         = opr;
	LPIActivityTrace activityTrace = opr;
	ASSERT(trace);
	ASSERT(activityTrace);

	if (trace->traceable())
	{
		LPIOperationTrace       operationTrace       = opr;
		LPIActivityPatternTrace activityPatternTrace = opr;
		ASSERT(operationTrace);
		ASSERT(activityPatternTrace);
		getOStream() << "EB " << sim->getCurrentTime()
		             << " "   << operationTrace->traceOperId()
		             << " "   << trace->traceId()
		             << " "   << activityPatternTrace->tracePatternId()
		             << " "   << activityTrace->traceResourcesListNumbers(sim, false)
		             << std::endl << getEOL(); 
	}
	getOStream() << activityTrace->traceResourcesList('\0', sim) << getEOL();
}

void RDOTrace::writeAfterOperationEnd(CREF(LPIBaseOperation) opr, PTR(RDOSimulatorTrace) sim)
{
	if (!canTrace())
		return;

	LPITrace         trace         = opr;
	LPIActivityTrace activityTrace = opr;
	ASSERT(trace);
	ASSERT(activityTrace);

	if (trace->traceable())
	{
		LPIOperationTrace       operationTrace       = opr;
		LPIActivityPatternTrace activityPatternTrace = opr;
		ASSERT(operationTrace);
		ASSERT(activityPatternTrace);
		getOStream() << "EF " << sim->getCurrentTime() 
		             << " "   << operationTrace->traceOperId() 
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId() 
		             << " "   << activityTrace->traceResourcesListNumbers(sim, false)
		             << std::endl << getEOL();
	}
	getOStream() << activityTrace->traceResourcesList('\0', sim) << getEOL();
}

void RDOTrace::writeTraceBegin(RDOSimulatorTrace *sim)
{
	if (isNull()) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(RDOSimulatorTrace *sim)
{
	if (isNull()) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

void RDOTrace::writeTraceEnd(RDOSimulatorTrace *sim)
{
	if (isNull()) return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 2" << std::endl << getEOL();
}

void RDOTrace::writeStatus(PTR(RDOSimulatorTrace) sim, char* status)
{
	if (isNull()) return;

	// Статус
	getOStream() << "$Status = " << status << " " << sim->getCurrentTime() << std::endl << getEOL();

	// Статистика по поиску на графе
	IBaseOperationContainer::CIterator it = sim->m_metaLogic->begin();
	while (it != sim->m_metaLogic->end())
	{
		LPIDPTSearchTraceStatistics dp_stat = (*it);
		if (dp_stat)
		{
			LPITrace dp_trace = dp_stat;
			ASSERT(dp_trace);
			// Информация о точке
			getOStream() << std::endl << getEOL();
			getOStream() << "DPS_C"
			             << "  " << dp_trace->getTraceID()
			             << "  " << dp_stat->getCalcCnt()
			             << "  " << dp_stat->getCalcResFoundCnt()
			             << std::endl << getEOL();
			if (dp_stat->getCalcCnt())
			{
				// Время поиска
				double d_min = 0;
				double d_max = 0;
				double d_med = 0;
				dp_stat->getStatsDOUBLE(IDPTSearchTraceStatistics::ST_TIMES, d_min, d_max, d_med);
				getOStream() << rdo::format("DPS_TM %0.3f  %0.3f  %0.3f", d_med, d_min, d_max) << std::endl << getEOL();

				// Используемая память
				unsigned int ui_min = 0;
				unsigned int ui_max = 0;
				dp_stat->getStatsRUINT(IDPTSearchTraceStatistics::ST_MEMORY, ui_min, ui_max, d_med);
				getOStream() << rdo::format("DPS_ME %0.0f  %u  %u", d_med, ui_min, ui_max) << std::endl << getEOL();

				// Стоимость решения
				dp_stat->getStatsDOUBLE(IDPTSearchTraceStatistics::ST_COST, d_min, d_max, d_med);
				getOStream() << "DPS_CO"
				             << " "  << d_med
				             << "  " << d_min
				             << "  " << d_max
				             << std::endl << getEOL();

				// Количество раскрытых вершин
				dp_stat->getStatsRUINT(IDPTSearchTraceStatistics::ST_NODES_EXPENDED, ui_min, ui_max, d_med);
				getOStream() << rdo::format("DPS_TO %0.0f  %u  %u", d_med, ui_min, ui_max) << std::endl << getEOL();

				// Количество вершин в графе
				dp_stat->getStatsRUINT(IDPTSearchTraceStatistics::ST_NODES_IN_GRAPH, ui_min, ui_max, d_med);
				getOStream() << rdo::format("DPS_TT %0.0f  %u  %u", d_med, ui_min, ui_max) << std::endl << getEOL();

				// Количество включавшихся в граф вершин (вершины, соответствующие одному и тому же состоянию системы, могут включаться в граф неоднократно, если порождается вершина с меньшей стоимостью пути)
				dp_stat->getStatsRUINT(IDPTSearchTraceStatistics::ST_NODES, ui_min, ui_max, d_med);
				getOStream() << rdo::format("DPS_TI %0.0f  %u  %u", d_med, ui_min, ui_max) << std::endl << getEOL();

				// Общее количество порожденных вершин-преемников
				dp_stat->getStatsRUINT(IDPTSearchTraceStatistics::ST_NODES_FULL, ui_min, ui_max, d_med);
				getOStream() << rdo::format("DPS_TG %0.0f  %u  %u", d_med, ui_min, ui_max) << std::endl << getEOL();
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
	if (!canTrace())
		return;

	getOStream() << "V  "  << sim->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

// ----------------------------------------------------------------------------
// ---------- RDOPokazTrace
// ----------------------------------------------------------------------------
RDOPokazTrace::RDOPokazTrace(RDORuntime* runtime, bool trace):
	RDOTraceableObject(trace),
	RDORuntimeContainer(runtime),
	m_wasChanged(true)
{
}

void RDOPokazTrace::tracePokaz()
{
	if (!traceable() || !m_wasChanged)
		return;

	getRuntime()->getTracer()->writePokaz(getRuntime(), this);
	m_wasChanged = false;
	return;
}

} // namespace rdoRuntime;
