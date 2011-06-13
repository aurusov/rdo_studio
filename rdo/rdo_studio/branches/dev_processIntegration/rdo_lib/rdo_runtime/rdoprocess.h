/**
 @file      rdoprocess.h
 @authors   Урусов Андрей, Лущан Дмитрий, etc.
 @date      unknown
 @brief     Процесснные операторы РДО
 @indent    4T
 */

#ifndef RDOPROCESS_H
#define RDOPROCESS_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess_i.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
class RDOPROCBlock: public IPROCBlock, public IInit, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPROCBlock)
	QUERY_INTERFACE(IInit     )
QUERY_INTERFACE_END

friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

protected:
	LPIPROCProcess  m_process;
	TransactList    m_transacts;

	RDOPROCBlock(LPIPROCProcess process);
	virtual ~RDOPROCBlock() {}

	DECLARE_IPROCBlock;
	DECLARE_IInit;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCProcess: public RDOLogicSimple, public IPROCProcess, public RDOPatternPrior
{
DEFINE_IFACTORY(RDOPROCProcess)
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE       (IPROCProcess)
	QUERY_INTERFACE_PARENT(RDOLogic    )
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END
friend class RDOPROCBlock;

public:
	DECLARE_IPROCProcess;

protected:
	tstring                    m_name;
	LPIPROCProcess             m_parent;
	std::list<LPIPROCProcess>  m_child;

private:
	RDOPROCProcess(CREF(tstring) _name, PTR(RDOSimulator) sim);
	LPRDOResourceType            m_transactType;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCResource);

CLASS_PARENT_OF(RDOPROCTransact, RDOResource)
{
DECLARE_FACTORY(RDOPROCTransact);
public:
	LPRDOPROCResource getRes()
	{
		return m_res;
	}
	void setRes(CREF(LPRDOPROCResource) pResource)
	{
		m_res = pResource;
	}
	REF(LPIPROCBlock) getBlock()
	{
		return m_block;
	}
	void setBlock(CREF(LPIPROCBlock) block)
	{
		m_block = block;
	}
	RDOPROCTransact(PTR(RDORuntime) runtime, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	void next();

private:
	LPIPROCBlock       m_block;
	LPRDOPROCResource  m_res;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
CLASS_PARENT_OF(RDOPROCResource, RDOResource)
{
DECLARE_FACTORY(RDOPROCResource);
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
	RDOPROCResource(PTR(RDORuntime) runtime, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	std::string whoAreYou() {return "procRes";	}

protected:
	std::list<LPRDOPROCTransact> transacts;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation 
{
DEFINE_IFACTORY(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval( RDOSimulator* sim );

private:
	RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime)
		: RDOPROCBlock(process)
		, timeNext    (NULL   )
		, pTimeCalc   (pTime  )
	{}

	double     timeNext;
	LPRDOCalc  pTimeCalc;

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
struct runtime_for_Queue
{
	LPRDOResource rss; 
	int Id_param;
	RDOValue defaultValue;
};

struct parser_for_Queue
{
	int Id_res;
	int Id_param;
};

class RDOPROCBlockForQueue: public RDOPROCBlock
{
protected:
	RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par );

	parser_for_Queue  fromParser;
	runtime_for_Queue forRes;
	void _onStart( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCQueue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForQueue)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

public:
	static int getDefaultValue()  { return 0; }
	static std::string getQueueParamName(){ return "длина_очереди"; }

private:
	RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par)
		: RDOPROCBlockForQueue(process, From_Par)
	{}

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCDepart);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForQueue)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

public:
	static int getDefaultValue()  { return 0; }
	static std::string getDepartParamName(){ return "длина_очереди"; }

private:
	RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par)
		: RDOPROCBlockForQueue(process, From_Par)
	{}
	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeizes
// ----------------------------------------------------------------------------
struct runtime_for_Seize
{
	LPRDOPROCResource  rss;
	int                Id_param;
	RDOValue           enum_free;
	RDOValue           enum_buzy;
	RDOValue           enum_break;
};

struct parser_for_Seize
{
	int Id_res;
	int Id_param;
};

class RDOPROCBlockForSeize: public RDOPROCBlock
{
public:
	static std::string getStateParamName() {return "Состояние";}
	static std::string getStateEnumFree()  {return "Свободен"; }
	static std::string getStateEnumBuzy()  {return "Занят";    }

protected:
	RDOPROCBlockForSeize(LPIPROCProcess process, std::vector < parser_for_Seize > From_Par);

	std::vector < runtime_for_Seize > forRes;
	std::vector < parser_for_Seize > fromParser;
	void _onStart( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeizes
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCSeize);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForSeize)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

private:
	RDOPROCSeize(LPIPROCProcess process, std::vector < parser_for_Seize > From_Par)
		: RDOPROCBlockForSeize(process, From_Par)
	{
		static ruint g_index = 1;
		index = g_index++;
	}

	ruint index;

	virtual void transactGoIn (PTR(RDOPROCTransact) _transact);
	virtual void transactGoOut(PTR(RDOPROCTransact) _transact);

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCReleases
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCRelease);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForSeize)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

private:
	RDOPROCRelease(LPIPROCProcess process, std::vector < parser_for_Seize > From_Par)
		: RDOPROCBlockForSeize(process, From_Par)
	{
		static ruint g_index = 1;
		index = g_index++;
	}

	ruint index;

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCAdvance);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

protected:
	LPRDOCalc pDelayCalc;

	struct LeaveTr {
		LPRDOPROCTransact transact;
		double           timeLeave;
		LeaveTr(CREF(LPRDOPROCTransact) _transact, double _timeLeave):
			transact (_transact ),
			timeLeave(_timeLeave)
		{}
	};
	std::list< LeaveTr > leave_list;

private:
	RDOPROCAdvance(LPIPROCProcess process, CREF(LPRDOCalc) _pDelayCalc)
		: RDOPROCBlock(process    )
		, pDelayCalc  (_pDelayCalc)
	{}
	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCTerminate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	int getTerm() {return term;}

private:
	RDOPROCTerminate(LPIPROCProcess process, ruint _term)
		: RDOPROCBlock(process)
		, term        (_term  )
	{}
	const ruint term; 
	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
class RDOPROCAssign: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCAssign);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCAssign(LPIPROCProcess process, CREF(LPRDOCalc) pValue, int Id_res, int Id_param)
		: RDOPROCBlock(process )
		, pParamValue (pValue  )
		, t_resId     (Id_res  )
		, t_parId     (Id_param)
	{}

	LPRDOCalc pParamValue;
	int       t_resId;
	int       t_parId;

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDOPROCESS_H
