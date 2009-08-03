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
class RDOPROCBlock: public IPROCBlock
{
RDO_IOBJECT(RDOPROCBlock);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPROCBlock)
QUERY_INTERFACE_END

friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

public:
	typedef std::list<PTR(RDOPROCTransact)> TransactList;

protected:
	RDOPROCProcess* process;
	TransactList    transacts;

	RDOPROCBlock( RDOPROCProcess* _process );
	virtual ~RDOPROCBlock() {}

	DECLARE_IPROCBlock;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCTransact;

class RDOPROCProcess: public RDOLogic
{
friend class RDOPROCBlock;

public:
	RDOPROCProcess(CREF(tstring) _name, PTR(RDOSimulator) sim);

	void insertChild(PTR(RDOPROCProcess)  value   );
	void next       (PTR(RDOPROCTransact) transact);

protected:
	tstring                         m_name;
	PTR(RDOPROCProcess)             m_parent;
	std::list<PTR(RDOPROCProcess)>  m_child;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCResource;
class RDOPROCTransact: public RDOResource
{
//friend class RDOPROCProcess;

public:
	static int typeID;

	RDOPROCResource* getRes()
	{
		return res;
	}
	void setRes(RDOPROCResource *Res)
	{
		res = Res;
	}
	RDOPROCBlock* getBlock()
	{
		return block;
	}
	void setBlock(RDOPROCBlock* _block)
	{
		block = _block;
	}
	RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block );
	void next();

private:
	RDOPROCBlock* block;
	RDOPROCResource *res;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDOResource
{
friend class RDOPROCSeize;
friend class RDOPROCRelease;
protected: 
	std::list<RDOPROCTransact*> transacts;
public:
	RDOPROCResource( RDORuntime* _runtime, int _number, ruint type, bool _trace );
	std::string whoAreYou() {return "procRes";	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation 
{
RDO_IOBJECT(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval( RDOSimulator* sim );

private:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time )
		: RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time )
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
public:
	RDOPROCBlockForQueue( RDOPROCProcess* _process, parser_for_Queue From_Par );

protected:
	parser_for_Queue  fromParser;
	runtime_for_Queue forRes;
	virtual void onStart		  ( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue, public IBaseOperation
{
RDO_IOBJECT(RDOPROCQueue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

public:
	static int getDefaultValue()  { return 0; }
	static std::string getQueueParamName(){ return "длина_очереди"; }

private:
	RDOPROCQueue( RDOPROCProcess* _process, parser_for_Queue From_Par )
		: RDOPROCBlockForQueue( _process, From_Par )
	{}

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue, public IBaseOperation
{
RDO_IOBJECT(RDOPROCDepart);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

public:
	static int getDefaultValue()  { return 0; }
	static std::string getDepartParamName(){ return "длина_очереди"; }

private:
	RDOPROCDepart( RDOPROCProcess* _process, parser_for_Queue From_Par )
		: RDOPROCBlockForQueue( _process, From_Par )
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
	RDOPROCBlockForSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par );
	static std::string getStateParamName() {return "Состояние";}
	static std::string getStateEnumFree()  {return "Свободен"; }
	static std::string getStateEnumBuzy()  {return "Занят";    }

protected:
	std::vector < runtime_for_Seize > forRes;
	std::vector < parser_for_Seize > fromParser;
	virtual void onStart( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeizes
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize, public IBaseOperation
{
RDO_IOBJECT(RDOPROCSeize);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

public:
	virtual void TransactGoIn( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );

private:
	RDOPROCSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par )
		: RDOPROCBlockForSeize( _process, From_Par )
	{
		static ruint g_index = 1;
		index = g_index++;
	}

	ruint index;

	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCReleases
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize, public IBaseOperation
{
RDO_IOBJECT(RDOPROCRelease);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCRelease( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par )
		: RDOPROCBlockForSeize( _process, From_Par )
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
RDO_IOBJECT(RDOPROCAdvance);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
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
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc )
		: RDOPROCBlock( _process ), delayCalc( _delayCalc )
	{}
	DECLARE_IBaseOperation;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation
{
RDO_IOBJECT(RDOPROCTerminate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

public:
	int getTerm() {return term;}

private:
	RDOPROCTerminate(RDOPROCProcess* _process, ruint _term)
		: RDOPROCBlock( _process ), term(_term)
	{}
	const ruint term; 
	DECLARE_IBaseOperation;
};


// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
class RDOPROCAssign: public RDOPROCBlock, public IBaseOperation
{
RDO_IOBJECT(RDOPROCAssign);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCAssign( RDOPROCProcess* _process, RDOCalc* value, int Id_res, int Id_param )
		: RDOPROCBlock( _process ), paramValue( value ), t_resId( Id_res ), t_parId( Id_param )
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
