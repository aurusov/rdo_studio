/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_keyboard.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Клавиатурные операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_KEYBOARD_H_
#define _LIB_RUNTIME_KEYBOARD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_keyboard_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOKeyboard
  \brief     Клавиатурная операция
*/
class RDOKeyboard: public RDOOperation, public IKeyboard
{
DEFINE_IFACTORY(RDOKeyboard);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOOperation)
	QUERY_INTERFACE(IKeyboard)
QUERY_INTERFACE_END

private:
	RDOKeyboard(CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, rbool trace, CREF(tstring) name);
	RDOKeyboard(CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, rbool trace, CREF(LPRDOCalc) pCondition, CREF(tstring) name);
	virtual ~RDOKeyboard();

	rbool m_shift;
	rbool m_control;
	ruint m_scan_code;

	virtual rbool choiceFrom(CREF(LPRDORuntime) pRuntime);

	DECLARE_IKeyboard;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_KEYBOARD_H_
