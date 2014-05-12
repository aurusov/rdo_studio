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
DECLARE_FACTORY(RDOKeyboard);
private:
	RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const std::string& name);
	RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const LPRDOCalc& pCondition, const std::string& name);
	virtual ~RDOKeyboard();

	bool m_shift;
	bool m_control;
	std::size_t m_scan_code;

	virtual bool choiceFrom(const LPRDORuntime& pRuntime);

	DECLARE_IKeyboard;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_KEYBOARD_H_
