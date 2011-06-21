/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdopokaz_group_i.h
 * author   : Урусов Андрей
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_GROUP_I_H_
#define _RDOPOKAZ_GROUP_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_simulator.h"
#include "rdo_lib/rdo_runtime/rdopokaz_i.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- IPokazGroup
// ----------------------------------------------------------------------------
class IPokazGroup
{
public:
	virtual void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onAppend(CREF(LPIPokaz)                 pResult ) = 0;
};
#define DECLARE_IPokazGroup                                 \
	void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onAppend(CREF(LPIPokaz)                 pResult );

#endif //! _RDOPOKAZ_GROUP_I_H_
