/*
 * copyright: (c) RDO-Team, 2011
 * filename : statements.h
 * author   : Чирков Михаил, Дмитрий Лущан
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_STATEMENTS_H_
#define _RDOCALC_STATEMENTS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcNoChange
// ----------------------------------------------------------------------------
CALC(RDOCalcNoChange)
{
	DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcIf
// ----------------------------------------------------------------------------
CALC(RDOCalcIf)
{
	DECLARE_FACTORY(RDOCalcIf)
private:
	RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcIfElse
// ----------------------------------------------------------------------------
CALC(RDOCalcIfElse)
{
	DECLARE_FACTORY(RDOCalcIfElse)
private:
	RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pIfStatement;
	LPRDOCalc m_pElseStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFor
// ----------------------------------------------------------------------------
CALC(RDOCalcFor)
{
	DECLARE_FACTORY(RDOCalcFor)
private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunReturn
// ----------------------------------------------------------------------------
CALC(RDOCalcFunReturn)
{
	DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunBreak
// ----------------------------------------------------------------------------
CALC(RDOCalcFunBreak)
{
	DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_STATEMENTS_H_
