#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Открывающая скобка
PREDECLARE_POINTER(RDOCalcOpenBrace);
class RDOCalcOpenBrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECLARE_ICalc;
};

//! Закрывающая скобка
PREDECLARE_POINTER(RDOCalcCloseBrace);
class RDOCalcCloseBrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECLARE_ICalc;
};

//! Окончание функции
PREDECLARE_POINTER(RDOCalcFunEnd);
class RDOCalcFunEnd: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunEnd)
private:
	RDOCalcFunEnd();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
