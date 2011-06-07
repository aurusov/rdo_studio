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
#include "rdo_lib/rdo_runtime/rdo_resource_i.h"
//#include "rdo_lib/rdo_runtime/rdoprocess.h"

namespace rdoRuntime
{

class RDORuntime;
class RDOResource;
class RDOPROCTransact;

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
OBJECT(RDOResourceType) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
public:
	RDOResourceType(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeTransact) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
public:
	RDOResourceTypeTransact(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeProccess) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
public:
	RDOResourceTypeProccess(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

} // namespace rdoRuntime

#endif // RDO_RES_TYPE_H
