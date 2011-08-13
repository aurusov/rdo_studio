/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      rdo_resource.h
 * @authors   Урусов Андрей, Лущан Дмитрий
 * @date      16.04.2008
 * @brief     Ресурсы в runtime
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_RESOURCE_H_
#define _LIB_RUNTIME_RESOURCE_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
PREDECLARE_OBJECT_INTERFACE(IResourceType);

/******************************************************************************//**
 * @class   RDOResource
 * @brief   "обычные" ресурсы моделей
 * @details Ресурсы, которые могут быть релевантны активностям и
 * событиям, но не могут использоваться в процессах
 *********************************************************************************/
OBJECT(RDOResource) IS INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
friend class RDOResourceType;
public:
	/**
	 * @enum  ConvertStatus
	 * @brief статусы конверторов релевантных ресурсов
	 */
	enum ConvertStatus
	{
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary);
	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy);
	virtual ~RDOResource();

	rbool operator!= (REF(RDOResource) other);

	ConvertStatus               getState   (                           ) const;
	CREF(RDOValue)              getParam   (ruint index                ) const;
	rbool                       checkType  (ruint type                 ) const;
	rbool                       canFree    (                           ) const;
	CREF(LPIResourceType)       getResType (                           ) const;
	ruint                       getType    (                           ) const;
	virtual ruint               paramsCount(                           ) const;
	LPRDOResource               clone      (CREF(LPRDORuntime) pRuntime) const;
	CREF(std::vector<RDOValue>) getParams  (                           ) const;

	typedef std::vector<RDOValue>::const_iterator VCI;

	virtual void    appendParams        (CREF(VCI) from_begin, CREF(VCI) from_end);
	void            setRuntime          (CREF(LPRDORuntime) pRuntime             );
	void            makeTemporary       (rbool value                             );
	void            setState            (ConvertStatus value                     );
	tstring         traceResourceState  (char prefix, CREF(LPRDORuntime) pRuntime);
	REF(RDOValue)   getParamRaw         (ruint index                             );
	void            setParam            (ruint index, CREF(RDOValue) value       );
	tstring         getTypeId           ();
	tstring         traceParametersValue();
	virtual tstring whoAreYou           ();
	void            incRef              ();
	void            decRef              ();

protected:
	std::vector<RDOValue> m_params;
	rbool                 m_temporary;
	ConvertStatus         m_state;

private:
	ruint            m_type;
	ruint            m_referenceCount;
	LPIResourceType  m_resType;
	tstring          m_typeId;

	tstring traceTypeId();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_resource.inl"

#endif // _LIB_RUNTIME_RESOURCE_H_
