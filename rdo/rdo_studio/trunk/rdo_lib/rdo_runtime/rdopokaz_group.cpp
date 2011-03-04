/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdopokaz_group.cpp
 * author   : Урусов Андрей
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdopokaz_group.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokazGroup::RDOPMDPokazGroup(rbool autoStart)
	: m_state(autoStart ? RGS_START : RGS_STOP)
{}

RDOPMDPokazGroup::~RDOPMDPokazGroup()
{}

void RDOPMDPokazGroup::resetPokaz(PTR(RDOSimulator) pSimulator)
{
	if (m_state == RGS_STOP)
		return;

	STL_FOR_ALL(m_resultList, it)
	{
		(*it)->resetPokaz(pSimulator);
	}
}

void RDOPMDPokazGroup::checkPokaz(PTR(RDOSimulator) pSimulator)
{
	if (m_state == RGS_STOP)
		return;

	STL_FOR_ALL(m_resultList, it)
	{
		(*it)->checkPokaz(pSimulator);
	}
}

void RDOPMDPokazGroup::calcStat(PTR(RDOSimulator) pSimulator)
{
	if (m_state == RGS_STOP)
		return;

	STL_FOR_ALL(m_resultList, it)
	{
		(*it)->calcStat(pSimulator);
	}
}

void RDOPMDPokazGroup::onStart(PTR(rdoRuntime::RDOSimulator) pSimulator)
{
	m_state = RGS_START;
	resetPokaz(pSimulator);
}

void RDOPMDPokazGroup::onStop(PTR(rdoRuntime::RDOSimulator) pSimulator)
{
	calcStat(pSimulator);
	m_state = RGS_STOP;
}

void RDOPMDPokazGroup::onAppend(CREF(LPIPokaz) pResult)
{
	ASSERT(pResult);

	m_resultList.push_back(pResult);
}

CLOSE_RDO_RUNTIME_NAMESPACE
