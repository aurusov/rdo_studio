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
// ---------- RDOGetRelResParamCalc
// ----------------------------------------------------------------------------
CALC(RDOGetRelResParamCalc)
{
DECLARE_FACTORY(RDOGetRelResParamCalc)
private:
	RDOGetRelResParamCalc(int relNumb, int parNumb);

	int m_relNumb;
	int m_parNumb;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelResParamCalc
// ----------------------------------------------------------------------------
template <EqualType equalType>
class RDOSetRelResParamCalc: public RDOCalc
{
public:
	RDOSetRelResParamCalc(int relNumb, int parNumb, CREF(LPRDOCalc) pCalc = NULL);
	virtual ~RDOSetRelResParamCalc();

protected:
	int        m_relNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelResParamDiapCalc
// ----------------------------------------------------------------------------
CALC(RDOSetRelResParamDiapCalc)
{
DECLARE_FACTORY(RDOSetRelResParamDiapCalc)
private:
	RDOSetRelResParamDiapCalc(int relNumb, int parNumb, CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pCalc);

	int          m_relNumb;
	int          m_parNumb;
	LPRDOCalc    m_pCalc;
	RDOValue     m_min_value;
	RDOValue     m_max_value;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOEraseResRelCalc
// ----------------------------------------------------------------------------
CALC(RDOEraseResRelCalc)
{
DECLARE_FACTORY(RDOEraseResRelCalc)
public:
	CREF(tstring) getName() const;

private:
	RDOEraseResRelCalc(int rel_res_id, CREF(tstring) rel_res_name);

	int      m_rel_res_id;
	tstring  m_rel_res_name;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/relres.inl"

#endif //! _RDOCALC_RELRES_H_
