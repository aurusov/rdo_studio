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
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
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

void RDOPROCBlockForQueue::_onStart(CREF(LPRDORuntime) pRuntime)
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
void RDOPROCQueue::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCQueue::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
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

IBaseOperation::BOResult RDOPROCQueue::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	TRACE1("%7.1f QUEUE\n", pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCQueue::onStop (CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCQueue::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCQueue::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
void RDOPROCDepart::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCDepart::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
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

IBaseOperation::BOResult RDOPROCDepart::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	TRACE1("%7.1f DEPART\n", pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCDepart::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCDepart::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCDepart::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
