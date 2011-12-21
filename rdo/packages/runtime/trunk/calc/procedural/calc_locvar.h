/*!
  \copyright (c) RDO-Team, 2011
  \file      locvar.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Локальные переменные
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_LOC_VAR_H_
#define _LIB_RUNTIME_CALC_LOC_VAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/equaltype.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcCreateLocalVariable
  \brief   Создание локальной переменной
*/
CALC(RDOCalcCreateLocalVariable)
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcInitLocalVariable
  \brief   Инициализация локальной переменной
*/
CALC(RDOCalcInitLocalVariable)
{
DECLARE_FACTORY(RDOCalcInitLocalVariable)
private:
	RDOCalcInitLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pCalc);

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetLocalVariable
  \brief   Получение локальной переменной
*/
CALC(RDOCalcGetLocalVariable)
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcSetLocalVariable
  \brief   Сохранение локальной переменной
*/
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

/*!
  \class   RDOCalcLocalVariableList
  \brief   Список локальных переменных
*/
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

#endif // _LIB_RUNTIME_CALC_LOC_VAR_H_
