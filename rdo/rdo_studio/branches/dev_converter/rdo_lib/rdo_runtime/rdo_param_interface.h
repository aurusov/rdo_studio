/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_param_interface.h
 * author   : Лущан Дмитрий
 * date     : 09.12.2010
 * bref     : Интерфейс IParam
 * indent   : 4T
 */

#ifndef _RDO_PARAM_INTERFACE_H_
#define _RDO_PARAM_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

class IParam
{
public:
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam) = 0;
	virtual rbool endParam() = 0;
};
#define DECLARE_IParam \
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam); \
	virtual rbool endParam();

#endif //! _RDO_PARAM_INTERFACE_H_
