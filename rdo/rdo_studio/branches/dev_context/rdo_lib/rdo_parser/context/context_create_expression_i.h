/*
 * copyright: (c) RDO-Team, 2011
 * filename : context_create_expression_i.h
 * author   : Урусов Андрей
 * date     : 07.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_CREATE_EXPRESSION_I_H_
#define _RDOPARSER_CONTEXT_CREATE_EXPRESSION_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/expression.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextCreateExpression
// ----------------------------------------------------------------------------
OBJECT_INTERFACE(IContextCreateExpression)
{
DECLARE_FACTORY(IContextCreateExpression)
public:
	virtual LPExpression onCreateExpression(CREF(RDOValue) value) = 0;

protected:
	IContextCreateExpression()
	{}
	virtual ~IContextCreateExpression()
	{}
};
#define DECLARE_IContextCreateExpression \
	LPExpression onCreateExpression(CREF(RDOValue) value);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_CREATE_EXPRESSION_I_H_
