/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdopokaz_group.h
 * author   : Урусов Андрей
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_GROUP_H_
#define _RDOPOKAZ_GROUP_H_

// =========================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdopokaz_group_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokazGroup
// ----------------------------------------------------------------------------
class RDOPMDPokazGroup: public IPokaz, public IPokazGroup
{
DEFINE_IFACTORY(RDOPMDPokazGroup);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz     )
	QUERY_INTERFACE(IPokazGroup)
QUERY_INTERFACE_END

private:
	RDOPMDPokazGroup(CREF(tstring) name);
	virtual ~RDOPMDPokazGroup();

	enum State
	{
		RGS_STOP,
		RGS_START
	};

	typedef std::vector<LPIPokaz> ResultList;
	ResultList  m_resultList;
	State       m_state;
	tstring     m_name;
	double      m_timeStart;

	DECLARE_IPokaz;
	DECLARE_IPokazGroup;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOPOKAZ_GROUP_H_
