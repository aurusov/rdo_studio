#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

class IResult: public virtual rdo::counter_reference
{
public:
	virtual void resetResult(const rdo::runtime::LPRDORuntime& Runtime) = 0;
	virtual void checkResult(const rdo::runtime::LPRDORuntime& Runtime) = 0;
	virtual void calcStat   (const rdo::runtime::LPRDORuntime& Runtime, std::ostream& stream) = 0;
};
DECLARE_POINTER(IResult);

#define DECLARE_IResult                                                  \
	virtual void resetResult(const rdo::runtime::LPRDORuntime& Runtime); \
	virtual void checkResult(const rdo::runtime::LPRDORuntime& Runtime); \
	virtual void calcStat   (const rdo::runtime::LPRDORuntime& Runtime, std::ostream& stream);

class IResultWatchQuant: public virtual rdo::counter_reference
{
public:
	virtual void setLogicCalc(const rdo::runtime::LPRDOCalc& pLogicCalc) = 0;
};
DECLARE_POINTER(IResultWatchQuant)

#define DECLARE_IResultWatchQuant \
	virtual void setLogicCalc(const rdo::runtime::LPRDOCalc& pLogicCalc);

class IResultWatchValue: public virtual rdo::counter_reference
{
public:
	virtual void checkResourceErased(const rdo::runtime::LPRDOResource& pResource  ) = 0;
	virtual void setLogicCalc       (const rdo::runtime::LPRDOCalc&     pLogicCalc ) = 0;
	virtual void setArithmCalc      (const rdo::runtime::LPRDOCalc&     pArithmCalc) = 0;
};
DECLARE_POINTER(IResultWatchValue);

#define DECLARE_IResultWatchValue                                                     \
	virtual void checkResourceErased(const rdo::runtime::LPRDOResource& pResource  ); \
	virtual void setLogicCalc       (const rdo::runtime::LPRDOCalc&     pLogicCalc ); \
	virtual void setArithmCalc      (const rdo::runtime::LPRDOCalc&     pArithmCalc);

class IResultGetValue: public virtual rdo::counter_reference
{
public:
	virtual const rdo::runtime::RDOValue& getValue() const = 0;
};
DECLARE_POINTER(IResultGetValue)

#define DECLARE_IResultGetValue \
	const rdo::runtime::RDOValue& getValue() const;
