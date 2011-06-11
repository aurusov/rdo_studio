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
	IS  INSTANCE_OF(IResourceType     )
	AND INSTANCE_OF(RDORuntimeObject  )
	AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
private:
	RDOResourceType(rsint number, PTR(RDORuntimeParent) pParent = NULL);
	DECLARE_IResourceType;

	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeTransact)
	IS  INSTANCE_OF(IResourceType     )
	AND INSTANCE_OF(RDORuntimeObject  )
	AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
private:
	RDOResourceTypeTransact(rsint number, PTR(RDORuntimeParent) pParent = NULL);
	DECLARE_IResourceType;

	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeProccess)
	IS  INSTANCE_OF(IResourceType     )
	AND INSTANCE_OF(RDORuntimeObject  )
	AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
private:
	RDOResourceTypeProccess(rsint number, PTR(RDORuntimeParent) pParent = NULL);
	DECLARE_IResourceType;

	rsint m_id;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDO_RES_TYPE_H
