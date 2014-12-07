#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcOpenBrace);
class RDOCalcOpenBrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcCloseBrace);
class RDOCalcCloseBrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcFunEnd);
class RDOCalcFunEnd: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunEnd)
private:
	RDOCalcFunEnd();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
