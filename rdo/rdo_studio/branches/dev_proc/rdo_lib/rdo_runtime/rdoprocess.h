#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

class RDOCalc;

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
class RDOPROCBlock: public RDOBaseOperation
{
friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

protected:
	RDOPROCProcess* process;
	std::list< RDOPROCTransact* > transacts;

	RDOPROCBlock( RDOPROCProcess* _process );
	virtual ~RDOPROCBlock() {}

public:
	virtual void TransactGoIn( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCTransact;

class RDOPROCProcess: public RDOLogic
{
friend class RDOPROCBlock;

protected:
	std::string                  name;
	RDOPROCProcess*              parent;
	std::list< RDOPROCProcess* > child;

public:
	RDOPROCProcess( const std::string& _name, RDOSimulator* sim );
	void insertChild( RDOPROCProcess* value );

	void next( RDOPROCTransact* transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCResource;
class RDOPROCTransact: public RDOResource
{
friend class RDOPROCProcess;

public:
	static int typeID;
protected:
	RDOPROCBlock* block;
	RDOPROCResource *res;
public:
	RDOPROCResource* getRes()
	{
		return res;
	}
	void setRes(RDOPROCResource *Res)
	{
		res = Res;
	}
	RDOPROCBlock* getBlock();
	RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block );
	void next();
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDOResource
{
friend class RDOPROCSeize;
friend class RDOPROCSeizes;
protected: 
	std::list<RDOPROCTransact*> transacts;
public:
	RDOPROCResource( RDORuntime* _runtime, int _number, unsigned int type, bool _trace );
	std::string whoAreYou() {return "procRes";	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock
{
protected:
	double   timeNext;
	RDOCalc* timeCalc;

protected:
	virtual void     onStart         ( RDOSimulator* sim );
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time ): 
	  RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time ) {}
	void calcNextTimeInterval( RDOSimulator* sim );
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
	parser_for_Queue  fromParser;
	runtime_for_Queue forRes;
	virtual void onStart		  ( RDOSimulator* sim );

public:
	RDOPROCBlockForQueue( RDOPROCProcess* _process, parser_for_Queue From_Par );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue
{
protected:
	virtual bool onCheckCondition ( RDOSimulator* sim );
	virtual BOResult onDoOperation( RDOSimulator* sim );

public:
	RDOPROCQueue( RDOPROCProcess* _process, parser_for_Queue From_Par ): 
		RDOPROCBlockForQueue( _process, From_Par ){}
	static int getDefaultValue()  { return 0; }
	static std::string getQueueParamName(){ return "длина_очереди"; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue
{
protected:
	virtual bool onCheckCondition ( RDOSimulator* sim );
	virtual BOResult onDoOperation( RDOSimulator* sim );

public:
	RDOPROCDepart( RDOPROCProcess* _process, parser_for_Queue From_Par ): 
		RDOPROCBlockForQueue( _process, From_Par ){}
	static int getDefaultValue()  { return 0; }
	static std::string getDepartParamName(){ return "длина_очереди"; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
struct runtime_for_Seize
{
	RDOPROCResource* rss; 
	int Id_param;
	RDOValue     enum_free;
	RDOValue     enum_buzy;
};
struct parser_for_Seize
{
	int Id_res;
	int Id_param;
};
class RDOPROCBlockForSeize: public RDOPROCBlock
{
protected:
	runtime_for_Seize forRes;
	parser_for_Seize  fromParser;
	virtual void onStart( RDOSimulator* sim );

public:
	RDOPROCBlockForSeize( RDOPROCProcess* _process, parser_for_Seize From_Par );
	static std::string getStateParamName() {return "Состояние";}
	static std::string getStateEnumFree()  {return "Свободен"; }
	static std::string getStateEnumBuzy()  {return "Занят";    }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
private:
	unsigned int index;
	virtual bool onCheckCondition ( RDOSimulator* sim );
	virtual BOResult onDoOperation( RDOSimulator* sim );

public:
	RDOPROCSeize( RDOPROCProcess* _process, parser_for_Seize From_Par ): 
		RDOPROCBlockForSeize( _process, From_Par )
	{
		static unsigned int g_index = 1;
		index = g_index++;
	}
	virtual void TransactGoIn ( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
private:
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCRelease( RDOPROCProcess* _process, parser_for_Seize From_Par ): 
	  RDOPROCBlockForSeize( _process, From_Par ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeizes
// ----------------------------------------------------------------------------

class RDOPROCBlockForSeizes: public RDOPROCBlock
{
protected:
	std::vector < runtime_for_Seize > forRes;
	std::vector < parser_for_Seize > fromParser;
	virtual void onStart( RDOSimulator* sim );

public:
	RDOPROCBlockForSeizes( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par );
	static std::string getStateParamName() {return "Состояние";}
	static std::string getStateEnumFree()  {return "Свободен"; }
	static std::string getStateEnumBuzy()  {return "Занят";    }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeizes
// ----------------------------------------------------------------------------
class RDOPROCSeizes: public RDOPROCBlockForSeizes
{
private:
	unsigned int index;
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCSeizes( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ): 
	  RDOPROCBlockForSeizes( _process, From_Par ) 
	  {
	  static unsigned int g_index = 1;
	  index = g_index++;
	  }
	virtual void TransactGoIn( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCReleases
// ----------------------------------------------------------------------------
class RDOPROCReleases: public RDOPROCBlockForSeizes
{
private:
	unsigned int index;
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCReleases( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ): 
	  RDOPROCBlockForSeizes( _process, From_Par ) 
	  {
	  static unsigned int g_index = 1;
	  index = g_index++;
	  }
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCBlock
{
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

	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc ): RDOPROCBlock( _process ), delayCalc( _delayCalc ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock
{
protected:
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation	 ( RDOSimulator* sim );
	const unsigned int term; 
public:
	int getTerm(){return term;}
	RDOPROCTerminate( RDOPROCProcess* _process, const unsigned int& _term ): RDOPROCBlock( _process ), term(_term) {}
};

} // namespace rdoRuntime

#endif // RDOPROCESS_H
