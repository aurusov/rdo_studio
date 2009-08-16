#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"
#include "rdo_runtime.h"
#include "rdoprocess_interface.h"

namespace rdoRuntime {

class RDOCalc;

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
class RDOPROCTransact;

class RDOPROCProcess: public RDOLogic, public IPROCProcess
{
DEFINE_FACTORY(RDOPROCProcess)
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOLogic    )
	QUERY_INTERFACE       (IPROCProcess)
QUERY_INTERFACE_END
friend class RDOPROCBlock;

protected:
	tstring                    m_name;
	LPIPROCProcess             m_parent;
	std::list<LPIPROCProcess>  m_child;

private:
	RDOPROCProcess(CREF(tstring) _name, PTR(RDOSimulator) sim);

	DECLARE_IPROCProcess;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCResource;
class RDOPROCTransact: public RDOResource
{
public:
	static int typeID;

	PTR(RDOPROCResource) getRes()
	{
		return m_res;
	}
	void setRes(PTR(RDOPROCResource) res)
	{
		m_res = res;
	}
	REF(LPIPROCBlock) getBlock()
	{
		return m_block;
	}
	void setBlock(CREF(LPIPROCBlock) block)
	{
		m_block = block;
	}
	RDOPROCTransact(PTR(RDOSimulator) sim, CREF(LPIPROCBlock) block);
	void next();

private:
	LPIPROCBlock         m_block;
	PTR(RDOPROCResource) m_res;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDOResource
{
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
	RDOPROCResource( RDORuntime* _runtime, int _number, ruint type, bool _trace );
	std::string whoAreYou() {return "procRes";	}

protected: 
	std::list<RDOPROCTransact*> transacts;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation 
{
DEFINE_FACTORY(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval( RDOSimulator* sim );

private:
	RDOPROCGenerate(LPIPROCProcess process, PTR(RDOCalc) time)
		: RDOPROCBlock(process)
		, timeNext    (NULL   )
		, timeCalc    (time   )
	{}

	double       timeNext;
	PTR(RDOCalc) timeCalc;

	DECLARE_IBaseOperation;
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
struct runtime_for_Queue
{
	RDOResource* rss; 
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
DEFINE_FACTORY(RDOPROCQueue);
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
DEFINE_FACTORY(RDOPROCDepart);
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
	RDOPROCResource* rss; 
	int Id_param;
	RDOValue     enum_free;
	RDOValue     enum_buzy;
	RDOValue	 enum_break;
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
DEFINE_FACTORY(RDOPROCSeize);
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
DEFINE_FACTORY(RDOPROCRelease);
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
DEFINE_FACTORY(RDOPROCAdvance);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

protected:
	RDOCalc* delayCalc;

	struct LeaveTr {
		RDOPROCTransact* transact;
		double           timeLeave;
		LeaveTr( RDOPROCTransact* _transact, double _timeLeave ):
			transact ( _transact ),
			timeLeave( _timeLeave )
		{
		}
	};
	std::list< LeaveTr > leave_list;

private:
	RDOPROCAdvance(LPIPROCProcess process, RDOCalc* _delayCalc)
		: RDOPROCBlock(process   )
		, delayCalc   (_delayCalc)
	{}
	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation
{
DEFINE_FACTORY(RDOPROCTerminate);
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
DEFINE_FACTORY(RDOPROCAssign);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCAssign(LPIPROCProcess process, RDOCalc* value, int Id_res, int Id_param)
		: RDOPROCBlock(process )
		, paramValue  (value   )
		, t_resId     (Id_res  )
		, t_parId     (Id_param)
	{
		int i = 0;
	}

	RDOCalc* paramValue;
	int t_resId;
	int t_parId;

	DECLARE_IBaseOperation;
};

} // namespace rdoRuntime

#endif // RDOPROCESS_H
