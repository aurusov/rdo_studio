/*
 * copyright: (c) RDO-Team, 2011
 * filename : braces.h
 * author   : Чирков Михаил
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_BRACES_H_
#define _RDOCALC_BRACES_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcOpenBrace
// ----------------------------------------------------------------------------
CALC(RDOCalcOpenBrace)
{
	DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCloseBrace
// ----------------------------------------------------------------------------
CALC(RDOCalcCloseBrace)
{
	DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcList
// ----------------------------------------------------------------------------
CALC(RDOCalcList)
{
	DECLARE_FACTORY(RDOCalcList)
public:
	typedef std::vector<LPRDOCalc> CalcList;

	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcList();

	CalcList m_calcList;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunList
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcFunList, RDOFunCalc)
{
	DECLARE_FACTORY(RDOCalcFunList)
public:
	typedef std::vector<LPRDOCalc> CalcFunList;

	void addFunCalc(CREF(LPRDOCalc) pCalc);
	void addRetCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunList();

	CalcFunList m_calcFunList;

	DECALRE_ICalc;
};


CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_BRACES_H_
