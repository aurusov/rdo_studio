/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_activity_i.h
 * @authors   Урусов Андрей
 * @date      01.08.2009
 * @brief     Интерфейс IActivity
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_ACTIVITY_I_H_
#define _RDO_ACTIVITY_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ********************************************************************************

class IActivity
{
public:
	virtual void addParamCalc  (CREF(rdoRuntime::LPRDOCalc) pCalc) = 0;
	virtual int  getResByRelRes(ruint rel_res_id) const            = 0;
	virtual void setRelRes     (ruint rel_res_id, ruint res_id)    = 0;
};
#define DECLARE_IActivity \
	virtual void addParamCalc  (CREF(rdoRuntime::LPRDOCalc) pCalc); \
	virtual int  getResByRelRes(ruint rel_res_id) const;            \
	virtual void setRelRes     (ruint rel_res_id, ruint res_id);

class IActivityTrace
{
public:
	virtual tstring traceResourcesList       (char prefix, CREF(rdoRuntime::LPRDORuntime) pRuntime)             = 0;
	virtual tstring traceResourcesListNumbers(CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool show_create_index) = 0;
};
#define DECLARE_IActivityTrace                                                                                   \
	virtual tstring traceResourcesList       (char prefix, CREF(rdoRuntime::LPRDORuntime) pRuntime);             \
	virtual tstring traceResourcesListNumbers(CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool show_create_index);

class IActivityPatternTrace
{
public:
	virtual CREF(tstring) tracePatternId() const = 0;
};
#define DECLARE_IActivityPatternTrace \
	virtual CREF(tstring) tracePatternId() const;

#endif // _RDO_ACTIVITY_I_H_
