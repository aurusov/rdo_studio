/*!
  \copyright (c) RDO-Team, 2011
  \file      context_switch_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.12.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_SWITCH_I_H_
#define _RDOPARSER_CONTEXT_SWITCH_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextSwitch
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextSwitch)
{
DECLARE_FACTORY(IContextSwitch)
public:
	virtual Context::FindResult onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const = 0;

protected:
	IContextSwitch()
	{}
	virtual ~IContextSwitch()
	{}
};
#define DECLARE_IContextSwitch \
	Context::FindResult onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const;

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_CONTEXT_SWITCH_I_H_
