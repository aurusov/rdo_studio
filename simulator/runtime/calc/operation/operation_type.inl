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
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- OperatorType
// --------------------------------------------------------------------------------
template <OperatorType::Type>
inline std::size_t& OperatorType::getCalcCounter()
{
	static std::size_t s_counter = 0;
	return s_counter;
}

CLOSE_RDO_RUNTIME_NAMESPACE
