#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"
#include "rdoruntime.h"

namespace rdoRuntime {

class RDORuntime;
class RDOCalc;

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCTransact;

class RDOPROCProcess: public RDOBaseOperation
{
friend class RDOPROCBlock;

protected:
	std::string                  name;
	RDOPROCProcess*              parent;
	std::list< RDOPROCProcess* > child;
	std::list< RDOPROCBlock* >   blocks;

public:
	RDOPROCProcess( const std::string& _name, RDORuntime* sim );
	void insertChild( RDOPROCProcess* value );
	virtual bool checkOperation( RDOSimulator* sim );

	void preProcess( RDOSimulator* sim );
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
	RDOPROCTransact( RDORuntime* sim, RDOPROCBlock* _block );
	void next();
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
class RDOPROCBlock
{
friend class RDOPROCTransact;
friend class RDOPROCProcess;

protected:
	RDOPROCProcess* process;
	std::list< RDOPROCTransact* > transacts;

	RDOPROCBlock( RDOPROCProcess* _process );
	virtual ~RDOPROCBlock() {}

public:
	virtual bool checkOperation( RDORuntime* sim ) = 0;
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
	virtual bool checkOperation( RDORuntime* sim );

public:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time ): RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time ) {}
	void calcNextTimeInterval( RDORuntime* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlock
{
protected:
	int rss_id;

public:
	RDOPROCSeize( RDOPROCProcess* _process, int _rss_id ): RDOPROCBlock( _process ), rss_id( _rss_id ) {}
	virtual bool checkOperation( RDORuntime* sim );
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

public:
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc ): RDOPROCBlock( _process ), delayCalc( _delayCalc ) {}
	virtual bool checkOperation( RDORuntime* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock
{
public:
	RDOPROCTerminate( RDOPROCProcess* _process ): RDOPROCBlock( _process ) {}
	virtual bool checkOperation( RDORuntime* sim );
};

} // rdoRuntime

#endif // RDOPROCESS_H
