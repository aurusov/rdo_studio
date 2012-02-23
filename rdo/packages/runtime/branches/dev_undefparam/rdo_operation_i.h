/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_operation_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.07.2009
  \brief     Интерфейс операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_OPERATION_I_H_
#define _LIB_RUNTIME_OPERATION_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IOperation
  \brief     Интерфейс модифицированного продукционного правила
*/
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

/*!
  \interface IOperationTrace
  \brief     Интерфейс IOperationTrace
*/
class IOperationTrace
{
public:
	virtual tstring traceOperId() const = 0;
};

#define DECLARE_IOperationTrace \
	virtual tstring traceOperId() const;

#endif // _LIB_RUNTIME_OPERATION_I_H_
