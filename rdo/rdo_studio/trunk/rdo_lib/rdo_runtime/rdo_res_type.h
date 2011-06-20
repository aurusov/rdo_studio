/**
 @file    rdo_res_type.h
 @authors Урусов Андрей, Лущан Дмитрий
 @date    07.06.2011
 @brief   Типы ресурсов в runtime
 @indent  4T
 */

#ifndef RDO_RES_TYPE_H
#define RDO_RES_TYPE_H

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_res_type_i.h"

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
class RDOResource;
class RDOPROCTransact;

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
OBJECT(RDOResourceType)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
friend class RDOCalcCreateResource;
friend class RDOPROCGenerate;
private:
	RDOResourceType(rsint number);
	virtual ~RDOResourceType();

	rsint m_id;

	DECLARE_IResourceType;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeTransact)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
private:
	RDOResourceTypeTransact(rsint number);
	virtual ~RDOResourceTypeTransact();

	rsint m_id;

	DECLARE_IResourceType;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeProccess)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
private:
	RDOResourceTypeProccess(rsint number);
	virtual ~RDOResourceTypeProccess();

	rsint m_id;

	DECLARE_IResourceType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDO_RES_TYPE_H
