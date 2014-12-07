#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

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

class IActivityPatternTrace: public virtual rdo::counter_reference
{
public:
	virtual const std::string& tracePatternId() const = 0;
};
DECLARE_POINTER(IActivityPatternTrace)

#define DECLARE_IActivityPatternTrace \
	virtual const std::string& tracePatternId() const;
