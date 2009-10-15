/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_keyboard_interface.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_KEYBOARD_INTERFACE_H_
#define _RDO_KEYBOARD_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IKeyboard
{
public:
	enum AddHotKeyResult
	{
		addhk_ok,
		addhk_already,
		addhk_notfound,
		addhk_dont
	};
	virtual rbool           hasHotKey() const = 0;
	virtual AddHotKeyResult addHotKey(PTR(rdoRuntime::RDORuntime) runtime, CREF(tstring) hotKey) = 0;
};
#define DECLARE_IKeyboard \
	virtual rbool           hasHotKey() const; \
	virtual AddHotKeyResult addHotKey(PTR(rdoRuntime::RDORuntime) runtime, CREF(tstring) hotKey);

#endif //! _RDO_KEYBOARD_INTERFACE_H_
