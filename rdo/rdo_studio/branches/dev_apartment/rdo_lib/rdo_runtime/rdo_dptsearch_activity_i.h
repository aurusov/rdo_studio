/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_dptsearch_activity_i.h
 * author   : Урусов Андрей
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_DPTSEARCH_ACTIVITY_I_H_
#define _RDO_DPTSEARCH_ACTIVITY_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_rule_i.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;

CLOSE_RDO_RUNTIME_NAMESPACE

class IDPTSearchActivity
{
public:
	enum ValueTime
	{
		vt_before,
		vt_after
	};

	virtual REF(LPIRule) rule     ()                                  = 0;
	virtual double       cost     (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual ValueTime    valueTime() const                            = 0;
};
#define DECLARE_IDPTSearchActivity                                     \
	virtual REF(LPIRule) rule     ();                                  \
	virtual double       cost     (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual ValueTime    valueTime() const;

#endif //! _RDO_DPTSEARCH_ACTIVITY_I_H_
