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

protected:
	RDOPROCProcess* process;
	std::list< RDOPROCTransact* > transacts;

	RDOPROCBlock( RDOPROCProcess* _process );
	virtual ~RDOPROCBlock() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCTransact;

class RDOPROCProcess: public RDOBaseLogic<RDOPROCBlock>
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
class RDOPROCTransact: public RDOResource
{
friend class RDOPROCProcess;

protected:
	RDOPROCBlock* block;

public:
	static int typeID;

	RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block );
	void next();
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDOResource
{
public:
	RDOPROCResource( RDORuntime* _runtime, int _number, unsigned int type, bool _trace );
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
	virtual void init( RDOSimulator* sim );
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );

public:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time ): RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time ) {}
	void calcNextTimeInterval( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
class RDOPROCBlockForSeize: public RDOPROCBlock
{
protected:
	RDOResource* rss;
	int          rss_id;
	RDOValue     enum_free;
	RDOValue     enum_buzy;
	virtual void init( RDOSimulator* sim );

public:
	RDOPROCBlockForSeize( RDOPROCProcess* _process, int _rss_id );

	static std::string getStateParamName() { return "Состояние"; }
	static std::string getStateEnumFree()  { return "Свободен";  }
	static std::string getStateEnumBuzy()  { return "Занят";     }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
private:
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );

public:
	RDOPROCSeize( RDOPROCProcess* _process, int _rss_id ): RDOPROCBlockForSeize( _process, _rss_id ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
private:
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );

public:
	RDOPROCRelease( RDOPROCProcess* _process, int _rss_id ): RDOPROCBlockForSeize( _process, _rss_id ) {}
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
			transact( _transact ),
			timeLeave( _timeLeave )
		{
		}
	};
	std::list< LeaveTr > leave_list;

	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );

public:
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc ): RDOPROCBlock( _process ), delayCalc( _delayCalc ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock
{
protected:
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation( RDOSimulator* sim );

public:
	RDOPROCTerminate( RDOPROCProcess* _process ): RDOPROCBlock( _process ) {}
};

} // namespace rdoRuntime

#endif // RDOPROCESS_H
