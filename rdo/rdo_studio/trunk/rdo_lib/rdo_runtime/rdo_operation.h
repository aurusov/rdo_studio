/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_operation.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_OPERATION_H_
#define _RDO_OPERATION_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
#include "rdo_lib/rdo_runtime/rdo_operation_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
class RDOOperation: public IBaseOperation, public IOperation, public RDOActivityPattern<RDOPatternOperation>, public RDOPatternPrior, public IOperationTrace
{
typedef RDOActivityPattern<RDOPatternOperation> pattern_type;
DEFINE_IFACTORY(RDOOperation);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IOperation)
	QUERY_INTERFACE(IOperationTrace)
QUERY_INTERFACE_END

friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

protected:
	RDOOperation( CREF(LPRDORuntime) pRuntime, RDOPatternOperation* pattern, bool trace, const std::string& name );
	RDOOperation( CREF(LPRDORuntime) pRuntime, RDOPatternOperation* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name );
	virtual ~RDOOperation();
	DECLARE_IOperation;

private:
	RDOOperation(CREF(LPRDORuntime) pRuntime, CREF(RDOOperation) originForClone);

	typedef std::vector<LPIOperation> CloneList;
	CloneList m_cloneList;

	bool      haveAdditionalCondition;
	LPRDOCalc additionalCondition;

	int m_operId;

	DECLARE_IBaseOperation;
	DECLARE_IOperationTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_OPERATION_H_
