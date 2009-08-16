/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_activity_interface.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_ACTIVITY_INTERFACE_H_
#define _RDO_ACTIVITY_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
#include <namespace.h>
// ===============================================================================

//OPEN_RDO_RUNTIME_NAMESPACE
//class RDOSimulatorTrace;
//CLOSE_RDO_RUNTIME_NAMESPACE

class IActivity
{
public:
	virtual void addParamCalc  (PTR(rdoRuntime::RDOCalc) calc)  = 0;
	virtual int  getResByRelRes(ruint rel_res_id) const         = 0;
	virtual void setRelRes     (ruint rel_res_id, ruint res_id) = 0;
};
#define DECLARE_IActivity \
	virtual void addParamCalc  (PTR(rdoRuntime::RDOCalc) calc);  \
	virtual int  getResByRelRes(ruint rel_res_id) const;         \
	virtual void setRelRes     (ruint rel_res_id, ruint res_id);

class IActivityTrace
{
public:
	virtual tstring traceResourcesList       (char prefix, PTR(rdoRuntime::RDOSimulatorTrace) sim)             = 0;
	virtual tstring traceResourcesListNumbers(PTR(rdoRuntime::RDOSimulatorTrace) sim, rbool show_create_index) = 0;
};
#define DECLARE_IActivityTrace \
	virtual tstring traceResourcesList       (char prefix, PTR(rdoRuntime::RDOSimulatorTrace) sim); \
	virtual tstring traceResourcesListNumbers(PTR(rdoRuntime::RDOSimulatorTrace) sim, rbool show_create_index);

class IActivityPatternTrace
{
public:
	virtual CREF(tstring) tracePatternId() const = 0;
};
#define DECLARE_IActivityPatternTrace \
	virtual CREF(tstring) tracePatternId() const;

#endif //! _RDO_PRIORITY_INTERFACE_H_
