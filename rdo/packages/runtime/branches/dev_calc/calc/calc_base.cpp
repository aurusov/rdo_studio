/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_base.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Вычислитель
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
RDOCalc::RDOCalc()
{}

RDOCalc::~RDOCalc()
{}

void RDOCalc::calcValue(CREF(LPRDORuntime) pRuntime)
{
	try
	{
#ifdef _DEBUG
/*
		if (src_text().empty())
		{
			TRACE(_T("%d\n"), sizeof(tstring));
		}
		else if (src_text().length() < 500)
		{
			TRACE(_T("calc: %s\n"), src_text().c_str());
			if (src_text() == _T("класс"))
			{
				TRACE(_T("calc: %s\n"), src_text().c_str());
			}
		}
		else 
		{
			tstring str = src_text();
			str.resize(500);
			TRACE(_T("calc: %s\n"), str.c_str());
		}
*/
#endif
		doCalc(pRuntime);
	}
	catch (CREF(RDORuntimeException) ex)
	{
		tstring message = rdo::format(_T("<Модельное время: %f>, '%s'"), pRuntime->getTimeNow(), m_srcInfo.src_text().c_str());
		if (!ex.message().empty())
		{
			message = rdo::format(_T("%s: %s"), message.c_str(), ex.message().c_str());
		}
		rdo::service::simulation::RDOSyntaxError error(
			rdo::service::simulation::RDOSyntaxError::UNKNOWN,
			message,
			m_srcInfo.src_pos().m_last_line,
			m_srcInfo.src_pos().m_last_pos,
			m_srcInfo.src_filetype()
		);
		pRuntime->error().push(error);
	}
}

CREF(RDOSrcInfo) RDOCalc::srcInfo() const
{
	return m_srcInfo;
}

void RDOCalc::setSrcInfo(CREF(RDOSrcInfo) srcInfo)
{
	m_srcInfo = srcInfo;
}

CLOSE_RDO_RUNTIME_NAMESPACE
