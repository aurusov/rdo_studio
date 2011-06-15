/**
 @file    rdo_resource.h
 @authors Урусов Андрей, Лущан Дмитрий
 @date    03.06.2011
 @brief   Ресурсы в runtime
 @indent  4T
 */

#ifndef RDO_RESOURCE_H
#define RDO_RESOURCE_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
PREDECLARE_OBJECT_INTERFACE(IResourceType);

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
OBJECT(RDOResource) IS INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject) AND INSTANCE_OF(RDORuntimeContainer)
{
friend class RDOResourceType;
public:
	enum ConvertStatus {
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	RDOResource(PTR(RDORuntime) runtime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary);
	RDOResource(PTR(RDORuntime) runtime, CREF(RDOResource) copy);
	virtual ~RDOResource();

	void setRuntime(RDORuntime* runtime);

	ConvertStatus         getState   (           ) const;
	CREF(RDOValue)        getParam   (ruint index) const;
	rbool                 checkType  (ruint type ) const;
	rbool                 canFree    (           ) const;
	CREF(LPIResourceType) getResType (           ) const;
	ruint                 getType    (           ) const;
	virtual ruint         paramsCount(           ) const;
	LPRDOResource         clone      (PTR(RDORuntime) runtime) const;
	CREF(std::vector<RDOValue>) getParams(       ) const;

	void            makeTemporary       (rbool value                            );
	void            setState            (ConvertStatus value                    );
	tstring         traceResourceState  (char prefix, PTR(RDOSimulatorTrace) sim);
	REF(RDOValue)   getParamRaw         (ruint index                            );
	void            setParam            (ruint index, CREF(RDOValue) value      );
	virtual void    appendParams        (const std::vector<RDOValue>::const_iterator& from_begin, const std::vector<RDOValue>::const_iterator& from_end);
	tstring         getTypeId           ();
	tstring         traceParametersValue();
	virtual tstring whoAreYou           ();
	void            incRef              ();
	void            decRef              ();
	bool operator!= (REF(RDOResource) other);

protected:
	std::vector<RDOValue> m_params;
	bool                  m_temporary;
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

#endif // RDO_RESOURCE_H
