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
#include "utils/src/interface/rdointerface.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
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
	virtual bool hasHotKey() const = 0;
	virtual AddHotKeyResult addHotKey(const rdo::runtime::LPRDORuntime& pRuntime, const std::string& hotKey) = 0;
};

#define DECLARE_IKeyboard \
	virtual bool hasHotKey() const; \
	virtual AddHotKeyResult addHotKey(const rdo::runtime::LPRDORuntime& pRuntime, const std::string& hotKey);

#endif // _LIB_RUNTIME_KEYBOARD_I_H_
