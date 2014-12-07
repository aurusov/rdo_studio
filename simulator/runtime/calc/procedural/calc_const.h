#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcConst);
class RDOCalcConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcConst)
public:
	//! Возвращает хранимую константу, может быть использован в parser-time
	//! \result Значение константы
	const RDOValue& getValue() const;

private:
	//! Инициализируется указанным значением переменную базового класса \ref RDOCalc::m_value
	//! \param value - Значение константы
	RDOCalcConst(const RDOValue& value);

	RDOValue m_value;

	//! Возвращает значение константы
	//! \param pRuntime - Не используется
	//! \result Значение константы
	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
