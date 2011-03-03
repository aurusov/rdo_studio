/*
 * copyright: (c) RDO-Team, 2010
 * filename : context.cpp
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/context/context_find_i.h"
#include "rdo_lib/rdo_parser/context/context_create_calc_i.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

Context::Context()
{}

Context::~Context()
{}

void Context::setContextStack(CREF(LPContextStack) pContextStack)
{
	ASSERT(pContextStack   );
	ASSERT(!m_pContextStack);
	m_pContextStack = pContextStack;
}

LPContext Context::find(CREF(tstring) name) const
{
	LPContext pThis(const_cast<PTR(Context)>(this));
	LPIContextFind pThisContextFind = pThis.interface_dynamic_cast<IContextFind>();
	if (pThisContextFind)
	{
		LPContext pThisResult = pThisContextFind->find(name);
		if (pThisResult)
		{
			return pThisResult;
		}
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->find(name) : LPContext();
}

rdoRuntime::LPRDOCalc Context::create(CREF(tstring) name)
{
	LPContext pThis(this);
	LPIContextCreateCalc pThisContextCreateCalc = pThis.interface_dynamic_cast<IContextCreateCalc>();
	if (pThisContextCreateCalc)
	{
		rdoRuntime::LPRDOCalc pCalc = pThisContextCreateCalc->create(name);
		if (pCalc)
		{
			return pCalc;
		}
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->create(name) : rdoRuntime::LPRDOCalc();
}

CLOSE_RDO_PARSER_NAMESPACE
