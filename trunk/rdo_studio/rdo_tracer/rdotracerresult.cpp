#include "stdafx.h"
#include "rdotracerresult.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerResult
// ----------------------------------------------------------------------------
RDOTracerResult::RDOTracerResult( const RDOTracerResultKind kind )
	: RDOTracerSerie( RDOST_RESULT ),
	resultKind( kind ),
	id( 0 )
{
}

RDOTracerResult::~RDOTracerResult()
{
}
