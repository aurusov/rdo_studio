/*
 * @copyright (c) RDO-Team, 2010
 * @file      rdo_param_i.h
 * @authors   Лущан Дмитрий
 * @date      09.12.2010
 * @brief     Интерфейс IParam
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_PARAM_I_H_
#define _RDO_PARAM_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
// ********************************************************************************

class IParam
{
public:
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam) = 0;
	virtual rbool endParam() = 0;
};
#define DECLARE_IParam \
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam); \
	virtual rbool endParam();

#endif // _RDO_PARAM_I_H_
