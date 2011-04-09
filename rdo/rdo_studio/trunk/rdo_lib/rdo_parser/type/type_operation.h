/*
 * copyright: (c) RDO-Team, 2011
 * filename : type_operation.h
 * author   : Урусов Андрей
 * date     : 10.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_OPERATION_H_
#define _RDOPARSER_TYPE_OPERATION_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

enum TypeValueOperation
{
	TVO_PLUS,
	TVO_MINUS,
	TVO_MULT,
	TVO_DIV,
	TVO_EQUAL,
	TVO_NOT_EQUAL,
	TVO_LESS,
	TVO_GREATER,
	TVO_LEQ,
	TVO_GEQ
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_OPERATION_H_
