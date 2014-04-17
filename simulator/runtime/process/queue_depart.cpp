/*!
  \copyright (c) RDO-Team, 2012
  \file      queue_depart.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессные операторы QUEUE и DEPART
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/process/queue_depart.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForQueue::_onStart(const LPRDORuntime& pRuntime)
{
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	LPRDOResource res = pRuntime->getResourceByID(Id_res);
	forRes.Id_param = Id_param;
	forRes.rss = res.object_static_cast<RDOPROCResource>();
	forRes.defaultValue = RDOValue(RDOPROCQueue::getDefaultValue());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
void RDOPROCQueue::onStart(const LPRDORuntime& pRuntime)
{
	_onStart(pRuntime);
}

bool RDOPROCQueue::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
	if (!m_transacts.empty())
	{
		RDOValue i = forRes.rss->getParam(forRes.Id_param);
		RDOValue j = RDOValue(int (1));
		forRes.rss->setParam(forRes.Id_param, i + j);
		return true;
	}
	else
	{
		return false;
	}
}

IBaseOperation::BOResult RDOPROCQueue::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCQueue::onStop (const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCQueue::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
void RDOPROCDepart::onStart(const LPRDORuntime& pRuntime)
{
	_onStart(pRuntime);
}

bool RDOPROCDepart::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
	if (!m_transacts.empty())
	{
		RDOValue i = forRes.rss->getParam(forRes.Id_param);
		RDOValue j = RDOValue(int (1));
		forRes.rss->setParam(forRes.Id_param, i - j);
		return true;
	}
	else
	{
		return false;
	}
}

IBaseOperation::BOResult RDOPROCDepart::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCDepart::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCDepart::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
