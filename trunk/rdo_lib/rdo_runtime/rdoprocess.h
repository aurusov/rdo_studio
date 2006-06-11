#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperation
// ----------------------------------------------------------------------------
class RDOSimulator;

class RDOPROCOperation: public RDOBaseOperation
{
public:
	RDOPROCOperation(): RDOBaseOperation() {}
	virtual ~RDOPROCOperation()            {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperation
{
public:
	RDOPROCGenerate(): RDOPROCOperation() {}
	virtual ~RDOPROCGenerate()            {}
	virtual bool checkOperation( RDOSimulator* sim );
};

#endif // RDOPROCESS_H
