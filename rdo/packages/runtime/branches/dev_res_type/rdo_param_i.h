/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_param_i.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      09.12.2010
  \brief     Интерфейс IParam
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PARAM_I_H_
#define _LIB_RUNTIME_PARAM_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalc)

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IParam
  \brief     Интерфейс для параметров
  \todo      перевести все параметры в РДО на этот интерфейс
*/
class IParam
{
public:
	virtual rbool setParam(CREF(rdo::runtime::LPRDOCalc) pParam) = 0;
	virtual rbool endParam() = 0;
};

#define DECLARE_IParam \
	virtual rbool setParam(CREF(rdo::runtime::LPRDOCalc) pParam); \
	virtual rbool endParam();

#endif // _LIB_RUNTIME_PARAM_I_H_
