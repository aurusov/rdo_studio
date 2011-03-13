/*
 * copyright: (c) RDO-Team, 2011
 * filename : std_fun.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
#include <math.h>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/std_fun.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DECLARE_RDO_STD_FUN_DOUBLE(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getInt()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getInt()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getInt(), runtime->getFuncArgument(1).getInt()); \
	return m_value; \
}

double modf(double value)
{
	double tmp;
	return ::modf(value, &tmp);
}

double log2(double value)
{
	return log(value) / log(2.0);
}

double logN(double value1, double value2)
{
	return log(value1) / log(value2);
}

int round(double value)
{
	return (int)floor(value + 0.5);
}

DECLARE_RDO_STD_FUN_DOUBLE       (Sin     , sin      );
DECLARE_RDO_STD_FUN_DOUBLE       (Cos     , cos      );
DECLARE_RDO_STD_FUN_DOUBLE       (Tan     , tan      );
DECLARE_RDO_STD_FUN_DOUBLE       (Cotan   , 1.0 / tan);
DECLARE_RDO_STD_FUN_DOUBLE       (ArcCos  , acos     );
DECLARE_RDO_STD_FUN_DOUBLE       (ArcSin  , asin     );
DECLARE_RDO_STD_FUN_DOUBLE       (ArcTan  , atan     );
DECLARE_RDO_STD_FUN_DOUBLE       (Abs     , fabs     );
DECLARE_RDO_STD_FUN_DOUBLE       (Sqrt    , sqrt     );
DECLARE_RDO_STD_FUN_DOUBLE       (Round   , round    );
DECLARE_RDO_STD_FUN_DOUBLE       (Exp     , exp      );
DECLARE_RDO_STD_FUN_DOUBLE       (Floor   , floor    );
DECLARE_RDO_STD_FUN_DOUBLE       (Frac    , modf     );
DECLARE_RDO_STD_FUN_DOUBLE       (Ln      , log      );
DECLARE_RDO_STD_FUN_DOUBLE       (Log10   , log10    );
DECLARE_RDO_STD_FUN_DOUBLE       (Log2    , log2     );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(LogN    , logN     );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Max     , max      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Min     , min      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Power   , pow      );
DECLARE_RDO_STD_FUN_INT          (IAbs    , abs      );
DECLARE_RDO_STD_FUN_INT_INT      (IMax    , max      );
DECLARE_RDO_STD_FUN_INT_INT      (IMin    , min      );
DECLARE_RDO_STD_FUN_INT          (Int     , int      );
DECLARE_RDO_STD_FUN_DOUBLE_INT   (IntPower, pow      );

CLOSE_RDO_RUNTIME_NAMESPACE
