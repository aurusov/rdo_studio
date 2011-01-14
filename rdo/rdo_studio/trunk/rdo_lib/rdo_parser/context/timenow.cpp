/*
 * copyright: (c) RDO-Team, 2010
 * filename : timenow.cpp
 * author   : Урусов Андрей
 * date     : 20.11.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/timenow.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextTimeNow
// ----------------------------------------------------------------------------

rdoRuntime::LPRDOCalc ContextTimeNow::getTimeNowCalc()
{
	if (!m_pCalc)
	{
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create();
		ASSERT(m_pCalc);
	}
	return m_pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
