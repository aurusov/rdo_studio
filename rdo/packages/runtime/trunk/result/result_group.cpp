/*!
  \copyright (c) RDO-Team, 2011
  \file      result_group.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Группы собираемых показателей
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/result/result_group.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_model_i.h"
#include "repository/rdorepository.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResultGroup::RDOPMDResultGroup(CREF(tstring) name)
	: m_name (name                               )
	, m_state(name.empty() ? RGS_START : RGS_STOP)
{}

RDOPMDResultGroup::~RDOPMDResultGroup()
{}

void RDOPMDResultGroup::resetResult(CREF(LPRDORuntime) pRuntime)
{
	if (m_state == RGS_STOP)
		return;

	ASSERT(pRuntime);

	m_timeStart = pRuntime->getCurrentTime();

	if (!m_name.empty())
	{
		if (!m_streamFull.is_open())
		{
			LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
			ASSERT(pThreadProxy);
			rdo::repository::RDOThreadRepository::CreateFileInfo file(rdo::format(_T("- %s - full"), m_name.c_str()), _T("txt"), m_streamFull);
			pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
		}
		if (!m_streamTable.is_open())
		{
			LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
			ASSERT(pThreadProxy);
			rdo::repository::RDOThreadRepository::CreateFileInfo file(rdo::format(_T("- %s - table"), m_name.c_str()), _T("txt"), m_streamTable);
			pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
			if (m_streamTable.is_open())
			{
				BOOST_FOREACH(const LPIResult& pResult, m_resultList)
				{
					LPIResultGetValue pGetValue = pResult;
					if (pGetValue)
					{
						LPIName pName = pGetValue;
						ASSERT(pName);
						m_streamTable << pName->name() << _T("\t");
					}
				}
				m_streamTable << std::endl;
			}
		}
	}

	BOOST_FOREACH(LPIResult& pResult, m_resultList)
	{
		pResult->resetResult(pRuntime);
	}
}

void RDOPMDResultGroup::checkResult(CREF(LPRDORuntime) pRuntime)
{
	if (m_state == RGS_STOP)
		return;

	BOOST_FOREACH(LPIResult& pResult, m_resultList)
	{
		pResult->checkResult(pRuntime);
	}
}

void RDOPMDResultGroup::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	if (m_state == RGS_STOP)
		return;

	if (!m_name.empty())
	{
		double timeStop = pRuntime->getCurrentTime();
		rdo::textstream textStream;
		textStream << rdo::format(_T("---> %s, %f -> %f = %f\n"), m_name.c_str(), m_timeStart, timeStop, timeStop - m_timeStart);
		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}
	}

	rbool tableWrite = false;
	BOOST_FOREACH(LPIResult& pResult, m_resultList)
	{
		rdo::textstream textStream;

		pResult->calcStat(pRuntime, textStream);

		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}

		LPIResultGetValue pGetValue = pResult;
		if (pGetValue)
		{
			if (pGetValue->getValue().typeID() != RDOType::t_real)
			{
				m_streamTable << pGetValue->getValue().getAsString() << _T("\t");
			}
			else
			{
				setlocale(LC_NUMERIC, "rus");
				m_streamTable << rdo::format(_T("%f"), pGetValue->getValue().getDouble()) << _T("\t");
				setlocale(LC_NUMERIC, "eng");
			}
			tableWrite = true;
		}
	}

	if (tableWrite)
	{
		m_streamTable << std::endl;
	}

	if (!m_name.empty())
	{
		rdo::textstream textStream;
		textStream << rdo::format(_T("<--- %s\n"), m_name.c_str());
		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}
	}
	pRuntime->getResults().flush();
}

void RDOPMDResultGroup::onStart(CREF(LPRDORuntime) pRuntime)
{
	m_state = RGS_START;
	resetResult(pRuntime);
}

void RDOPMDResultGroup::onStop(CREF(LPRDORuntime) pRuntime)
{
	calcStat(pRuntime, pRuntime->getResults().getOStream());
	m_state = RGS_STOP;
}

void RDOPMDResultGroup::onAppend(CREF(LPIResult) pResult)
{
	ASSERT(pResult);

	m_resultList.push_back(pResult);
}

CLOSE_RDO_RUNTIME_NAMESPACE
