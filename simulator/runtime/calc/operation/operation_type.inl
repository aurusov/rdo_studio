/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_type.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.07.2011
  \brief     Тип операции: арифметическое или логическое выражение
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- OperatorType
// --------------------------------------------------------------------------------
template <OperatorType::Type>
inline REF(ruint) OperatorType::getCalcCounter()
{
	static ruint s_counter = 0;
	return s_counter;
}

CLOSE_RDO_RUNTIME_NAMESPACE
