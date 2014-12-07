#ifndef _LIB_RUNTIME_LOGIC_H_
#define _LIB_RUNTIME_LOGIC_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_logic_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class Order>
class RDOLogic: public IBaseOperation, public IBaseOperationContainer, public ILogic
{
public:
	typedef  BaseOperationList                  ChildList;
	typedef  BaseOperationList::iterator        Iterator;
	typedef  BaseOperationList::const_iterator  CIterator;

protected:
	RDOLogic(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDOLogic();

	DECLARE_IBaseOperationContainer;

	LPRDOCalc m_pCondition;
	bool m_lastCondition;
	ChildList m_childList;
	LPIBaseOperation m_pFirst;
	LPIBaseOperationContainer m_pParent;
	bool m_multithreading;

private:
	bool checkSelfCondition(const LPRDORuntime& pRuntime);
	void start(const LPRDORuntime& pRuntime);
	void stop(const LPRDORuntime& pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_ILogic;
};

class RDOOrderSimple
{
public:
	static LPIBaseOperation sort(const LPRDORuntime& pRuntime, BaseOperationList& container);
};

class RDOOrderMeta
{
public:
	static LPIBaseOperation sort(const LPRDORuntime& pRuntime, BaseOperationList& container);
};

class RDOLogicSimple: public RDOLogic<RDOOrderSimple>
{
protected:
	RDOLogicSimple(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent)
		: RDOLogic<RDOOrderSimple>(pRuntime, pParent)
	{}
	virtual ~RDOLogicSimple()
	{}
};

class RDOLogicMeta: public RDOLogic<RDOOrderMeta>
{
DECLARE_FACTORY(RDOLogicMeta);
protected:
	RDOLogicMeta()
		: RDOLogic<RDOOrderMeta>(NULL)
	{}
	virtual ~RDOLogicMeta()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_logic-inl.h"

#endif // _LIB_RUNTIME_LOGIC_H_
