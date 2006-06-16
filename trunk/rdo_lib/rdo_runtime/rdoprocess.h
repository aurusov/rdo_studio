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
public:
	RDOPROCSeize( RDOPROCProcess* _process ): RDOPROCBlock( _process ) {}
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
