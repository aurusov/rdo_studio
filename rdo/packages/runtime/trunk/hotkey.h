/*!
  \copyright (c) RDO-Team, 2012
  \file      hotkey.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_HOTKEY_H_
#define _LIB_RUNTIME_HOTKEY_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Горячие клавиши
class RDOHotKeyToolkit
{
public:
	typedef  ruint                       KeyCode;
	typedef  std::map<tstring, KeyCode>  KeySet;
	typedef  KeySet::const_iterator      CIterator;

	enum    { UNDEFINED_KEY = ~0 };

	RDOHotKeyToolkit();
	KeyCode codeFromString(CREF(tstring) key) const;

private:
	KeySet  m_keyList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_HOTKEY_H_
