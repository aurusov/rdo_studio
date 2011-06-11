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
#include "rdo_common/rdointerface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);
CLOSE_RDO_RUNTIME_NAMESPACE

class IResourceType
{
public:
	virtual rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, bool trace) const = 0;
};
#define DECLARE_IResourceType \
	rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, bool trace) const;

#endif // RDO_RES_TYPE_I_H
