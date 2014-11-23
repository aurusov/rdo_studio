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
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

/*!
  \interface IActivity
  \brief     Интерфейс IActivity
*/
class IActivity: public virtual rdo::counter_reference
{
public:
	virtual void addParamCalc(const rdo::runtime::LPRDOCalc& pCalc) = 0;
	virtual int getResByRelRes(std::size_t rel_res_id) const = 0;
	virtual void setRelRes(std::size_t rel_res_id, std::size_t res_id) = 0;
};
DECLARE_POINTER(IActivity);

#define DECLARE_IActivity                                            \
	virtual void addParamCalc(const rdo::runtime::LPRDOCalc& pCalc); \
	virtual int getResByRelRes(std::size_t rel_res_id) const;        \
	virtual void setRelRes(std::size_t rel_res_id, std::size_t res_id);

/*!
  \interface IActivityTrace
  \brief     Интерфейс IActivityTrace
*/
class IActivityTrace: public virtual rdo::counter_reference
{
public:
	virtual std::string traceResourcesList(char prefix, const rdo::runtime::LPRDORuntime& pRuntime) = 0;
	virtual std::string traceResourcesListNumbers(const rdo::runtime::LPRDORuntime& pRuntime, bool show_create_index) = 0;
};
DECLARE_POINTER(IActivityTrace)

#define DECLARE_IActivityTrace                                                                      \
	virtual std::string traceResourcesList(char prefix, const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual std::string traceResourcesListNumbers(const rdo::runtime::LPRDORuntime& pRuntime, bool show_create_index);

/*!
  \interface IActivityPatternTrace
  \brief     Интерфейс IActivityPatternTrace
*/
class IActivityPatternTrace: public virtual rdo::counter_reference
{
public:
	virtual const std::string& tracePatternId() const = 0;
};
DECLARE_POINTER(IActivityPatternTrace)

#define DECLARE_IActivityPatternTrace \
	virtual const std::string& tracePatternId() const;

#endif // _LIB_RUNTIME_ACTIVITY_I_H_
