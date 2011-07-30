/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      locvar.h
 * @authors   Чирков Михаил
 * @date      03.12.10
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDOCALC_LOCVAR_H_
#define _RDOCALC_LOCVAR_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcCreateLocalVariable
// ********************************************************************************
CALC(RDOCalcCreateLocalVariable)
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcInitLocalVariable
// ********************************************************************************
CALC(RDOCalcInitLocalVariable)
{
DECLARE_FACTORY(RDOCalcInitLocalVariable)
private:
	RDOCalcInitLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pCalc);

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcGetLocalVariable
// ********************************************************************************
CALC(RDOCalcGetLocalVariable)
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcSetLocalVariable
// ********************************************************************************
template <EqualType equalType>
class RDOCalcSetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariable)
private:
	RDOCalcSetLocalVariable(CREF(tstring) name, LPRDOCalc pCalc = NULL);
	virtual ~RDOCalcSetLocalVariable();

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOCalcLocalVariableList
// ********************************************************************************
CALC(RDOCalcLocalVariableList)
{
	DECLARE_FACTORY(RDOCalcLocalVariableList)
public:
	typedef std::vector<LPRDOCalc> CalcLocalVariableList;

	void addCalcLocalVariable(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcLocalVariableList();

	CalcLocalVariableList m_calcLocalVariableList;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/locvar.inl"

#endif // _RDOCALC_LOCVAR_H_
