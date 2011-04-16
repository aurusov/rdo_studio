/*
 * copyright: (c) RDO-Team, 2011
 * filename : relres.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 28.12.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_RELRES_H_
#define _RDOCALC_RELRES_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
template <EqualType equalType>
class RDOSetRelParamCalc: public RDOCalc
{
public:
	RDOSetRelParamCalc(int relNumb, int parNumb, CREF(LPRDOCalc) pCalc = NULL);
	virtual ~RDOSetRelParamCalc();

protected:
	int        m_relNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/relres.inl"

#endif //! _RDOCALC_RELRES_H_
