/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_locvar.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Локальные переменные
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/equaltype.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Создание локальной переменной
CALC(RDOCalcCreateLocalVariable)
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pValueCalc);

	tstring    m_name;
	LPRDOCalc  m_pValueCalc;

	DECLARE_ICalc;
};

//! Получение значения локальной переменной
CALC(RDOCalcGetLocalVariable)
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECLARE_ICalc;
};

//! Установка значения локальной переменной
template <EqualType equalType>
class RDOCalcSetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariable)
private:
	RDOCalcSetLocalVariable(CREF(tstring) name, LPRDOCalc pCalc = NULL);
	virtual ~RDOCalcSetLocalVariable();

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

//! Список локальных переменных
CALC(RDOCalcLocalVariableList)
{
DECLARE_FACTORY(RDOCalcLocalVariableList)
public:
	typedef std::vector<LPRDOCalc> CalcLocalVariableList;

	void addCalcLocalVariable(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcLocalVariableList();

	CalcLocalVariableList m_calcLocalVariableList;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/procedural/calc_locvar.inl"

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
