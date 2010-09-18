/*
 * copyright: (c) RDO-Team, 2010
 * filename : timenow.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_CONTEXT_TIMENOW_H_
#define _CONVERTOR_RDOCONVERTER_CONTEXT_TIMENOW_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/context/context.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

CLASS(ContextTimeNow): IMPLEMENTATION_OF(Context)
{
DECLARE_FACTORY(ContextTimeNow);
private:
	rdoRuntime::LPRDOCalc m_pCalc;
	DECLARE_IConvertorContext;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#include "rdo_lib/rdo_converter/context/timenow.inl"

#endif //! _CONVERTOR_RDOCONVERTER_CONTEXT_TIMENOW_H_
