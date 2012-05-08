/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировщик
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic_dptsearch.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

#ifdef RDOSIM_COMPATIBLE
tstring doubleToString(double value)
{
	std::ostringstream _str;
	_str << value;
	tstring::size_type pos = _str.str().find("e");
	if (pos != tstring::npos)
	{
		tstring __str = _str.str();
		__str.erase(pos + 2, 1);
		return __str.c_str();
	}
	else
	{
		return _str.str().c_str();
	}
}
#else

double doubleToString(double value)
{
	return value;
}
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTrace - Формирует строки трассировки
// --------------------------------------------------------------------------------
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

void RDOTrace::writeSearchDecision(CREF(LPRDORuntime) pRuntime, PTR(TreeNode) node)
{
	if (!canTrace())
		return;

	LPIActivityTrace        activityTrace        = node->m_activity->rule();
	LPIActivityPatternTrace activityPatternTrace = node->m_activity->rule();
	LPITrace                trace                = node->m_activity;
	ASSERT(trace);
	ASSERT(activityTrace);
	ASSERT(activityPatternTrace);

	getOStream() << node->m_number
	             << " " << trace->traceId()
	             << " " << activityPatternTrace->tracePatternId()
	             << " " << activityTrace->traceResourcesListNumbers(pRuntime, true)
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
		CREF(LPRDORuntime)      pRuntime             = node->m_pRuntime;
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
		             << " " << activityTrace->traceResourcesListNumbers(pRuntime, true)
		             << std::endl << getEOL();

		PTR(RDODPTSearchTrace) dpTrace = static_cast<PTR(RDODPTSearchTrace)>(node->m_root->m_dp);
		if (dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all)
		{
			getOStream() << activityTrace->traceResourcesList('S', pRuntime) << getEOL();
		}
	}
}

void RDOTrace::writeSearchResult(char letter, CREF(LPRDORuntime) simTr, PTR(TreeRoot) treeRoot)
{
	if (!canTrace())
		return;


	boost::posix_time::ptime systime_current = boost::posix_time::microsec_clock::local_time();

	ruint msec_current = RDOSimulatorBase::getMSec(systime_current);
	ruint msec_begin   = RDOSimulatorBase::getMSec(treeRoot->m_ptime);
	double sec_delay = static_cast<double>(msec_current - msec_begin) / 1000;

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

void RDOTrace::writePermanentResources(CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) res_perm)
{
	if (!canTrace())
		return;

	getOStream() << traceResourcesList('\0', pRuntime, res_perm) << getEOL();
}

tstring RDOTrace::traceResourcesList(char prefix, CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) rel_res_list)
{
	tstring res;
	for (std::list<LPRDOResource>::const_iterator i = rel_res_list.begin(); i != rel_res_list.end(); ++i)
	{
		if (*i)
		{
			res += (*i)->traceResourceState(prefix, pRuntime);
		}
	}
	return res;
}

void RDOTrace::writeEvent(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime)
{
	if (!canTrace())
		return;

	LPIActivityTrace activityTrace = opr;
	ASSERT(activityTrace);

#ifdef RDOSIM_COMPATIBLE
	getOStream() << activityTrace->traceResourcesList('\0', pRuntime) << getEOL();
#endif

	LPITrace trace = opr;
	ASSERT(trace);

	if (trace->traceable())
	{
		LPIActivityPatternTrace activityPatternTrace = opr;
		ASSERT(activityPatternTrace);

		getOStream() << "EI " << pRuntime->getCurrentTime()
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId() 
		             << " "   << activityTrace->traceResourcesListNumbers(pRuntime, true)
		             << std::endl << getEOL();
	}

#ifndef RDOSIM_COMPATIBLE
	getOStream() << activityTrace->traceResourcesList('\0', pRuntime) << getEOL();
#endif
}

void RDOTrace::writeRule(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime)
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
		int operId = pRuntime->getFreeOperationId();
		getOStream() << "ER " << pRuntime->getCurrentTime()
		             << " "   << operId
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId()
		             << " "   << activityTrace->traceResourcesListNumbers(pRuntime, false)
		             << std::endl << getEOL();
		pRuntime->freeOperationId(operId);
	}
	getOStream() << activityTrace->traceResourcesList('\0', pRuntime) << getEOL();
}

void RDOTrace::writeAfterOperationBegin(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime)
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
		getOStream() << "EB " << pRuntime->getCurrentTime()
		             << " "   << operationTrace->traceOperId()
		             << " "   << trace->traceId()
		             << " "   << activityPatternTrace->tracePatternId()
		             << " "   << activityTrace->traceResourcesListNumbers(pRuntime, false)
		             << std::endl << getEOL(); 
	}
	getOStream() << activityTrace->traceResourcesList('\0', pRuntime) << getEOL();
}

void RDOTrace::writeAfterOperationEnd(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime)
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
		getOStream() << "EF " << pRuntime->getCurrentTime() 
		             << " "   << operationTrace->traceOperId() 
		             << " "   << trace->traceId() 
		             << " "   << activityPatternTrace->tracePatternId() 
		             << " "   << activityTrace->traceResourcesListNumbers(pRuntime, false)
		             << std::endl << getEOL();
	}
	getOStream() << activityTrace->traceResourcesList('\0', pRuntime) << getEOL();
}

void RDOTrace::writeTraceBegin(CREF(LPRDORuntime) pRuntime)
{
	if (isNull()) return;

	getOStream() << "ES " << pRuntime->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(CREF(LPRDORuntime) pRuntime)
{
	if (isNull()) return;

	getOStream() << "ES " << pRuntime->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

void RDOTrace::writeTraceEnd(CREF(LPRDORuntime) pRuntime)
{
	if (isNull()) return;

   getOStream() << "ES " << pRuntime->getCurrentTime() 
      << " 2" << std::endl << getEOL();
}

void RDOTrace::writeStatus(CREF(LPRDORuntime) pRuntime, CREF(tstring) status)
{
	if (isNull()) return;

	// Статус
	getOStream() << "$Status = " << status << " " << pRuntime->getCurrentTime() << std::endl << getEOL();

	// Статистика по поиску на графе
	IBaseOperationContainer::CIterator it = pRuntime->m_pMetaLogic->begin();
	while (it != pRuntime->m_pMetaLogic->end())
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
				ruint ui_min = 0;
				ruint ui_max = 0;
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
		++it;
	}

	// Используемая память
	getOStream() << std::endl << getEOL();
	getOStream() << "DPS_MM " << pRuntime->memory_get() << std::endl << getEOL();
}

void RDOTrace::writeResult(CREF(LPRDORuntime) pRuntime, PTR(RDOResultTrace) pok)
{
	if (!canTrace())
		return;

	getOStream() << "V  "  << pRuntime->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

// --------------------------------------------------------------------------------
// -------------------- RDOResultTrace
// --------------------------------------------------------------------------------
RDOResultTrace::RDOResultTrace(CREF(LPRDORuntime) pRuntime, rbool trace)
	: RDOTraceableObject(trace   )
	, m_pRuntime        (pRuntime)
	, m_wasChanged      (true    )
{
	ASSERT(m_pRuntime);
}

void RDOResultTrace::traceResult()
{
	if (!traceable() || !m_wasChanged)
		return;

	m_pRuntime->getTracer()->writeResult(m_pRuntime, this);
	m_wasChanged = false;
	return;
}

CLOSE_RDO_RUNTIME_NAMESPACE
