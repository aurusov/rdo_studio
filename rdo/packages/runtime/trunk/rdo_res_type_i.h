/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type_i.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      05.06.2011
  \brief     Интерфейс для типов ресурсов
  \details   Типы ресурсов - фабрики ресурсов. См. паттерн "Фабричный метод"
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_I_H_
#define _LIB_RUNTIME_RES_TYPE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/interface_ptr.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime );
PREDECLARE_POINTER(RDOResource);

/*!
  \interface IResourceType
  \brief     Предоставляет фабричный метод createRes()
*/
OBJECT_INTERFACE(IResourceType)
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdo::runtime::LPRDOResource createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag) = 0;

	typedef  RDOResource  value_type;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}
};

#define DECLARE_IResourceType \
	rdo::runtime::LPRDOResource createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_I_H_
