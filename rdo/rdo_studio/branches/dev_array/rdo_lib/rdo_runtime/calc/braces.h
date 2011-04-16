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
#include "rdo_lib/rdo_runtime/calc/statements.h"
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
// ---------- RDOCalcBodyBrace
// ----------------------------------------------------------------------------
CALC(RDOCalcBodyBrace)
{
	DECLARE_FACTORY(RDOCalcBodyBrace)
public:
	typedef std::vector<LPRDOCalc> CalcList;

	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcBodyBrace();

	CalcList m_calcList;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunBodyBrace
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcFunBodyBrace, RDOFunCalc)
{
	DECLARE_FACTORY(RDOCalcFunBodyBrace)
public:
	typedef std::vector<LPRDOCalc> CalcFunList;

	void addFunCalc(CREF(LPRDOCalc) pCalc);
	void addRetCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunBodyBrace();

	CalcFunList m_calcFunList;

	DECALRE_ICalc;
};


CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_BRACES_H_
