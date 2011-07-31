/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_operation_i.h
 * @authors   Урусов Андрей
 * @date      30.07.2009
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_OPERATION_I_H_
#define _LIB_RUNTIME_OPERATION_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
// ********************************************************************************

class IOperation
{
public:
	virtual void   onBeforeChoiceFrom    (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual rbool  choiceFrom            (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   onBeforeOperationBegin(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   convertBegin          (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   onAfterOperationBegin (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   onBeforeOperationEnd  (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   convertEnd            (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void   onAfterOperationEnd   (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual double getNextTimeInterval   (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
};
#define DECLARE_IOperation \
	virtual void   onBeforeChoiceFrom    (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual rbool  choiceFrom            (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   onBeforeOperationBegin(CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   convertBegin          (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   onAfterOperationBegin (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   onBeforeOperationEnd  (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   convertEnd            (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void   onAfterOperationEnd   (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual double getNextTimeInterval   (CREF(rdoRuntime::LPRDORuntime) pRuntime)

class IOperationTrace
{
public:
	virtual tstring traceOperId() const = 0;
};
#define DECLARE_IOperationTrace \
	virtual tstring traceOperId() const;

#endif // _LIB_RUNTIME_OPERATION_I_H_
