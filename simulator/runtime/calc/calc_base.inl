/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_base.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2009
  \brief     Вычислитель
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
inline bool RDOCalc::compare(const LPRDOCalc& /*pCalc*/) const
{
	return false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
