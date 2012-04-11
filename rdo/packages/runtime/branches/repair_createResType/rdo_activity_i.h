/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_activity_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.08.2009
  \brief     Интерфейс IActivity
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ACTIVITY_I_H_
#define _LIB_RUNTIME_ACTIVITY_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

/*!
  \interface IActivity
  \brief     Интерфейс IActivity
*/
class IActivity
{
public:
	virtual void addParamCalc  (CREF(rdo::runtime::LPRDOCalc) pCalc) = 0;
	virtual int  getResByRelRes(ruint rel_res_id) const              = 0;
	virtual void setRelRes     (ruint rel_res_id, ruint res_id)      = 0;
};

#define DECLARE_IActivity \
	virtual void addParamCalc  (CREF(rdo::runtime::LPRDOCalc) pCalc); \
	virtual int  getResByRelRes(ruint rel_res_id) const;              \
	virtual void setRelRes     (ruint rel_res_id, ruint res_id);

/*!
  \interface IActivityTrace
  \brief     Интерфейс IActivityTrace
*/
class IActivityTrace
{
public:
	virtual tstring traceResourcesList       (char prefix, CREF(rdo::runtime::LPRDORuntime) pRuntime)             = 0;
	virtual tstring traceResourcesListNumbers(CREF(rdo::runtime::LPRDORuntime) pRuntime, rbool show_create_index) = 0;
};

#define DECLARE_IActivityTrace                                                                                     \
	virtual tstring traceResourcesList       (char prefix, CREF(rdo::runtime::LPRDORuntime) pRuntime);             \
	virtual tstring traceResourcesListNumbers(CREF(rdo::runtime::LPRDORuntime) pRuntime, rbool show_create_index);

/*!
  \interface IActivityPatternTrace
  \brief     Интерфейс IActivityPatternTrace
*/
class IActivityPatternTrace
{
public:
	virtual CREF(tstring) tracePatternId() const = 0;
};

#define DECLARE_IActivityPatternTrace \
	virtual CREF(tstring) tracePatternId() const;

#endif // _LIB_RUNTIME_ACTIVITY_I_H_
