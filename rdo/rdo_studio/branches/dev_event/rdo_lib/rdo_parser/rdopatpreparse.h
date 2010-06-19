/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopatpreparse.h
 * author   : Ћущан ƒмитрий
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPATPREPARSE_H_
#define _RDOPATPREPARSE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdosmart_ptr.h"
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
class RDOEvent: public rdo::smart_ptr_counter_reference
{
DECLARE_FACTORY(RDOEvent);
public:
	typedef std::list<PTR(rdoRuntime::RDOCalcEventPlan)> CalcList;

	CREF(tstring)    name           () const;
	void             attachCalc     (PTR(rdoRuntime::RDOCalcEventPlan) pCalc);
	CREF(CalcList)   getCalcList    () const;
	LPIBaseOperation getRuntimeEvent() const;
	void             setRuntimeEvent(LPIBaseOperation pRuntimeEvent);

private:
	RDOEvent(CREF(tstring) name);
	virtual ~RDOEvent();

	tstring          m_name;
	CalcList         m_calcList;
	LPIBaseOperation m_runtimeEvent;
};

DECLARE_POINTER(RDOEvent);

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdopatpreparse.inl"

#endif // _RDOPATPREPARSE_H_
