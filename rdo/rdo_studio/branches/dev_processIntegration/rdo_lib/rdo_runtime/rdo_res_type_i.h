/**
 @file    rdo_res_type_i.h
 @authors Ћущан ƒмитрий
 @date    05.06.2011
 @brief   »нтерфейс дл€ типов ресурсов
 @indent  4T
 @details “ипы ресурсов представл€ют из себ€ фабрики, которые реализуют метод createRes дл€ создани€ ресурсов
 */

#ifndef RDO_RES_TYPE_I_H
#define RDO_RES_TYPE_I_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);

OBJECT_INTERFACE(IResourceType)
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, bool trace) const = 0;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}
};
#define DECLARE_IResourceType \
	rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, bool trace) const;

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDO_RES_TYPE_I_H
