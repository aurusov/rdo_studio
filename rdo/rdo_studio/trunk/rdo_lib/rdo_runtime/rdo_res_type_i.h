/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      rdo_res_type_i.h
 * @author    Лущан Дмитрий
 * @date      05.06.2011
 * @brief     Интерфейс для типов ресурсов
 * @details   Типы ресурсов - фабрики ресурсов. См. Паттерн "Фабричный метод"
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_RES_TYPE_I_
#define _LIB_RUNTIME_RES_TYPE_I_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);

/******************************************************************************//**
 * @interface IResourceType
 * @brief     Предоставляет фабричный метод createRes()
 *********************************************************************************/
OBJECT_INTERFACE(IResourceType)
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdoRuntime::LPRDOResource createRes(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool trace, rbool permanentFlag) = 0;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}
};

/******************************************************************************//**
 * @def   DECLARE_IResourceType
 * @brief Интерфейс типа ресурса - метод \a createRes()
 *********************************************************************************/
#define DECLARE_IResourceType \
	rdoRuntime::LPRDOResource createRes(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool trace, rbool permanentFlag);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_I_
