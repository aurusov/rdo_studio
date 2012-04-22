/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_keyboard_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.08.2009
  \brief     Интерфейс клавиатурной операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_KEYBOARD_I_H_
#define _LIB_RUNTIME_KEYBOARD_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IKeyboard
  \brief     Интерфейс клавиатурной операции
*/
class IKeyboard
{
public:
	//! Статуc нажатия клавиши
	enum AddHotKeyResult
	{
		addhk_ok,       //!< клавиша нажата
		addhk_already,  //!< клавиша уже нажата
		addhk_notfound, //!< клавиша не найдена
		addhk_dont	    //!< клавиша не нажата
	};
	virtual rbool           hasHotKey() const = 0;
	virtual AddHotKeyResult addHotKey(CREF(rdo::runtime::LPRDORuntime) pRuntime, CREF(tstring) hotKey) = 0;
};

#define DECLARE_IKeyboard \
	virtual rbool           hasHotKey() const; \
	virtual AddHotKeyResult addHotKey(CREF(rdo::runtime::LPRDORuntime) pRuntime, CREF(tstring) hotKey);

#endif // _LIB_RUNTIME_KEYBOARD_I_H_
