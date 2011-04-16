/*
 * copyright: (c) RDO-Team, 2010
 * filename : locvar.h
 * author   : Чирков Михаил
 * date     : 03.12.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_LOCVAR_H_
#define _RDOCALC_LOCVAR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcSetLocalVariableEqualType
// ----------------------------------------------------------------------------
template <EqualType equalType>
class RDOCalcSetLocalVariableEqualType: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariableEqualType)
private:
	RDOCalcSetLocalVariableEqualType(CREF(tstring) name, LPRDOCalc pCalc = NULL);
	virtual ~RDOCalcSetLocalVariableEqualType();

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/locvar.inl"

#endif //! _RDOCALC_LOCVAR_H_
