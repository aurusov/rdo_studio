#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"
#include "rdoruntime.h"

namespace rdoRuntime {

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

	virtual void init( RDOSimulator* sim );
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

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
	RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block );
	void next();
};

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
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock
{
protected:
	double   timeNext;
	RDOCalc* timeCalc;

protected:
	virtual void init( RDOSimulator* sim );
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

public:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time ): RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time ) {}
	void calcNextTimeInterval( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlock
{
protected:
	int rss_id;

	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

public:
	RDOPROCSeize( RDOPROCProcess* _process, int _rss_id ): RDOPROCBlock( _process ), rss_id( _rss_id ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlock
{
protected:
	int rss_id;

	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

public:
	RDOPROCRelease( RDOPROCProcess* _process, int _rss_id ): RDOPROCBlock( _process ), rss_id( _rss_id ) {}
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

	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

public:
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc ): RDOPROCBlock( _process ), delayCalc( _delayCalc ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock
{
protected:
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

public:
	RDOPROCTerminate( RDOPROCProcess* _process ): RDOPROCBlock( _process ) {}
};

} // rdoRuntime

#endif // RDOPROCESS_H
