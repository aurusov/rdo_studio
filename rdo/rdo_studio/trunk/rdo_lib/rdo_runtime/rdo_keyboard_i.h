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
#include "rdo_common/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface IKeyboard
  \brief     Интерфейс клавиатурной операции
*/
class IKeyboard
{
public:
	/*!
	  \enum  AddHotKeyResult
	  \brief Статуc нажатия клавиши
	  \var   addhk_ok        клавиша нажата
	  \var   addhk_already   клавиша уже нажата
	  \var   addhk_notfound  клавиша не найдена
	  \var   addhk_dont      клавиша не нажата
	*/
	enum AddHotKeyResult
	{
		addhk_ok,
		addhk_already,
		addhk_notfound,
		addhk_dont
	};
	virtual rbool           hasHotKey() const = 0;
	virtual AddHotKeyResult addHotKey(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(tstring) hotKey) = 0;
};

#define DECLARE_IKeyboard \
	virtual rbool           hasHotKey() const; \
	virtual AddHotKeyResult addHotKey(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(tstring) hotKey);

#endif // _LIB_RUNTIME_KEYBOARD_I_H_
