#ifndef _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_
#define _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_rule_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;

CLOSE_RDO_RUNTIME_NAMESPACE

class IDPTSearchActivity: public virtual rdo::counter_reference
{
public:
	enum ValueTime
	{
		vt_before,
		vt_after
	};

	virtual LPIRule& rule() = 0;
	virtual double cost(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
	virtual ValueTime valueTime() const = 0;
};
DECLARE_POINTER(IDPTSearchActivity)

#define DECLARE_IDPTSearchActivity                                   \
	virtual LPIRule& rule();                                         \
	virtual double cost(const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual ValueTime valueTime() const;

#endif // _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_
