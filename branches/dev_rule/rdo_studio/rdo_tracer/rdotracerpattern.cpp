#include "stdafx.h"
#include "rdotracerpattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTracerPattern
// ----------------------------------------------------------------------------
RDOTracerPattern::RDOTracerPattern( const RDOTracerPatternKind kind ) :
	RDOTracerTreeItem(),
	patKind( kind )
{
}

RDOTracerPattern::~RDOTracerPattern()
{
}