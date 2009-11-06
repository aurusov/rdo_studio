/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_H_
#define _RDO_LOGIC_H_

#pragma warning(disable : 4786)

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo.h"
#include "rdo_runtime_interface_registrator.h"
#include "rdo_logic_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalc;

// ----------------------------------------------------------------------------
// ---------- RDOLogicBase
// ----------------------------------------------------------------------------
template <class Order>
class RDOLogic: public IBaseOperation, public IBaseOperationContainer, public ILogic, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IBaseOperationContainer)
	QUERY_INTERFACE(ILogic)
QUERY_INTERFACE_END

public:
	typedef  BaseOperationList                  ChildList;
	typedef  BaseOperationList::iterator        Iterator;
	typedef  BaseOperationList::const_iterator  CIterator;

protected:
	RDOLogic();
	virtual ~RDOLogic();

	DECLARE_IBaseOperationContainer;

	PTR(RDOCalc)      m_condition;
	rbool             m_lastCondition;
	ChildList         m_childList;
	LPIBaseOperation  m_first;

private:
	rbool checkSelfCondition(PTR(RDOSimulator) sim);
	void  start             (PTR(RDOSimulator) sim);
	void  stop              (PTR(RDOSimulator) sim);

	DECLARE_IBaseOperation;
	DECLARE_ILogic;
};

// ----------------------------------------------------------------------------
// ---------- OrderFIFO
// ----------------------------------------------------------------------------
class OrderFIFO
{
public:
	static LPIBaseOperation sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOSimplePriorOrder
// ----------------------------------------------------------------------------
class RDOSimplePriorOrder
{
public:
	static LPIBaseOperation sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicFIFO
// ----------------------------------------------------------------------------
class RDOLogicFIFO: public RDOLogic<OrderFIFO>
{
protected:
	DEFINE_FACTORY(RDOLogicFIFO);

	RDOLogicFIFO()
		: RDOLogic<OrderFIFO>()
	{}
	virtual ~RDOLogicFIFO()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicSimplePrior
// ----------------------------------------------------------------------------
class RDOLogicSimplePrior: public RDOLogic<RDOSimplePriorOrder>
{
protected:
	DEFINE_FACTORY(RDOLogicSimplePrior);

	RDOLogicSimplePrior()
		: RDOLogic<RDOSimplePriorOrder>()
	{}
	virtual ~RDOLogicSimplePrior()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_logic.inl"

#endif //! _RDO_LOGIC_H_
