// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
RDOCalc::RDOCalc()
{}

RDOCalc::~RDOCalc()
{}

RDOValue RDOCalc::calcValue(const LPRDORuntime& pRuntime)
{
	try
	{
#ifdef _DEBUG
/*
		if (src_text().empty())
		{
			TRACE("%d\n", sizeof(std::string));
		}
		else if (src_text().length() < 500)
		{
			TRACE("calc: %s\n", src_text().c_str());
			if (src_text() == "класс")
			{
				TRACE("calc: %s\n", src_text().c_str());
			}
		}
		else 
		{
			std::string str = src_text();
			str.resize(500);
			TRACE("calc: %s\n", str.c_str());
		}
*/
#endif
		return doCalc(pRuntime);
	}
	catch (const RDOUndefinedException& ex)
	{
		throw ex;
	}
	catch (const RDORuntimeException& ex)
	{
		std::string message = rdo::format("<Модельное время: %f>, '%s'", pRuntime->getTimeNow(), m_srcInfo.src_text().c_str());
		if (!ex.message().empty())
		{
			message = rdo::format("%s: %s", message.c_str(), ex.message().c_str());
		}
		rdo::simulation::report::FileMessage error(
			message,
			m_srcInfo.src_filetype(),
			m_srcInfo.src_pos().m_last_line,
			m_srcInfo.src_pos().m_last_pos
		);
		pRuntime->error().push(error);
	}
	return RDOValue();
}

bool RDOCalc::compare(const LPRDOCalc& /*pCalc*/) const
{
	return false;
}

const RDOSrcInfo& RDOCalc::srcInfo() const
{
	return m_srcInfo;
}

void RDOCalc::setSrcInfo(const RDOSrcInfo& srcInfo)
{
	m_srcInfo = srcInfo;
}

CLOSE_RDO_RUNTIME_NAMESPACE
