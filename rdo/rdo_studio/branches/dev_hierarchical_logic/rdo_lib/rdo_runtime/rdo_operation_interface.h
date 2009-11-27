/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_operation_interface.h
 * author   : Урусов Андрей
 * date     : 30.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_OPERATION_INTERFACE_H_
#define _RDO_OPERATION_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IOperation
{
public:
	virtual void   onBeforeChoiceFrom    (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual bool   choiceFrom            (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onBeforeOperationBegin(PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   convertBegin          (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onAfterOperationBegin (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onBeforeOperationEnd  (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   convertEnd            (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onAfterOperationEnd   (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual double getNextTimeInterval   (PTR(rdoRuntime::RDOSimulator) sim) = 0;
};
#define DECLARE_IOperation \
	virtual void   onBeforeChoiceFrom    (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual bool   choiceFrom            (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onBeforeOperationBegin(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   convertBegin          (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onAfterOperationBegin (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onBeforeOperationEnd  (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   convertEnd            (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onAfterOperationEnd   (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual double getNextTimeInterval   (PTR(rdoRuntime::RDOSimulator) sim)

class IOperationTrace
{
public:
	virtual tstring traceOperId() const = 0;
};
#define DECLARE_IOperationTrace \
	virtual tstring traceOperId() const;

#endif //! _RDO_OPERATION_INTERFACE_H_
