/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_keyboard.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_KEYBOARD_H_
#define _RDO_KEYBOARD_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOKeyboard
// ----------------------------------------------------------------------------
class RDOKeyboard: public RDOOperation, public IKeyboard
{
DEFINE_IFACTORY(RDOKeyboard);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOOperation)
	QUERY_INTERFACE(IKeyboard)
QUERY_INTERFACE_END

private:
	RDOKeyboard( CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, bool trace, const std::string& name );
	RDOKeyboard( CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name );
	virtual ~RDOKeyboard();

	bool m_shift;
	bool m_control;
	int  m_scan_code;

	virtual bool choiceFrom( CREF(LPRDORuntime) pRuntime );

	DECLARE_IKeyboard;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_KEYBOARD_H_
