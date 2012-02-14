/*!
  \copyright (c) RDO-Team, 2011
  \file      context_create_calc_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_CREATE_CALC_I_H_
#define _RDOPARSER_CONTEXT_CREATE_CALC_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "utils/smart_ptr/factory.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextCreateCalc
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextCreateCalc)
{
DECLARE_FACTORY(IContextCreateCalc)
public:
	virtual rdoRuntime::LPRDOCalc create(CREF(RDOValue) name) = 0;

protected:
	IContextCreateCalc()
	{}
	virtual ~IContextCreateCalc()
	{}
};
#define DECLARE_IContextCreateCalc \
	rdoRuntime::LPRDOCalc create(CREF(RDOValue) name);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_CONTEXT_CREATE_CALC_I_H_
