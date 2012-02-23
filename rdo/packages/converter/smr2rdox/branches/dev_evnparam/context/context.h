/*!
  \copyright (c) RDO-Team, 2011
  \file      context.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_CONTEXT_H_
#define _CONVERTOR_RDOCONVERTER_CONTEXT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_value.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

class Converter;

// --------------------------------------------------------------------------------
// -------------------- IContext
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

S_INTERFACE(IContext)
{
	virtual LPContext             parser (PTR(Converter) pParser, CREF(RDOValue) value) = 0;
	virtual rdoRuntime::LPRDOCalc getCalc()                                             = 0;
};

#define DECLARE_IConvertorContext                                                \
	LPContext             parser (PTR(Converter) pParser, CREF(RDOValue) value); \
	rdoRuntime::LPRDOCalc getCalc();

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
OBJECT(Context) IS IMPLEMENTATION_OF(IContext)
{
public:
	virtual ~Context() {};
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_CONTEXT_H_
