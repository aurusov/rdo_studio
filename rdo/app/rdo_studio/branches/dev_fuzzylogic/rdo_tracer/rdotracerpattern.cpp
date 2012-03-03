#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerPattern
// --------------------------------------------------------------------------------
RDOTracerPattern::RDOTracerPattern( const RDOTracerPatternKind kind ) :
	RDOTracerTreeItem(),
	patKind( kind )
{
}

RDOTracerPattern::~RDOTracerPattern()
{
}