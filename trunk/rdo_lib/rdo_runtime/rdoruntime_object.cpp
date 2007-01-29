#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoruntime_object.h"

namespace rdoRuntime {

RDORuntimeObject::RDORuntimeObject( RDORuntime* _runtime ):
	runtime( _runtime ),
	parent( NULL )
{
//	parser->insertDeletables( this );
}

RDORuntimeObject::RDORuntimeObject( const RDORuntimeObject* _parent ):
	parent( _parent )
{
	runtime = _parent->runtime;
//	parser->insertDeletables( this );
}

RDORuntimeObject::~RDORuntimeObject()
{
//	parser->removeDeletables( this );
}

void RDORuntimeObject::reparent( const RDORuntimeObject* _parent )
{
	parent  = _parent;
	runtime = _parent->runtime;
}

} // namespace rdoRuntime
