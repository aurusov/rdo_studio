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
#include "rdo_common/rdomacros.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
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

#endif //! _CONVERTOR_RDOCONVERTER_CONTEXT_H_
