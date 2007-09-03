#include "pch.h"
#include "rdoruntime_object.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
size_t RDORuntimeObject::memory_size = 0;

RDORuntimeObject::RDORuntimeObject( RDORuntimeParent* _parent ):
	parent( _parent )
{
	if ( parent ) {
		parent->insertObject( this );
	} else {
//		TRACE( "========= no parent for %d\n", this );
	}
}

RDORuntimeObject::~RDORuntimeObject()
{
//	TRACE( "destroy %d\n", this );
	if ( parent ) {
		parent->removeObject( this );
	}
}

void RDORuntimeObject::detach()
{
	if ( parent ) {
		parent->removeObject( this );
		parent = NULL;
	}
}

void RDORuntimeObject::reparent( RDORuntimeParent* _parent )
{
	if ( parent != _parent ) {
		if ( parent ) {
			parent->removeObject( this );
		}
		parent = _parent;
		if ( parent ) {
			parent->insertObject( this );
		}
	}
}

RDORuntime* RDORuntimeObject::getRuntime() const
{
	if ( !parent ) return const_cast<RDORuntime*>(static_cast<const RDORuntime*>(this));
	RDORuntimeObject* _parent = parent;
	while ( _parent->parent ) {
		_parent = _parent->parent;
	}
	return static_cast<RDORuntime*>(_parent);
}

#ifndef _DEBUG
void* RDORuntimeObject::operator new( size_t sz )
{
	memory_size += sz;
	RDORuntimeObject* obj = static_cast<RDORuntimeObject*>(::operator new( sz ));
	obj->object_size = sz;
//	obj->runtime->memory_insert( sz );
	return obj;
}

void RDORuntimeObject::operator delete( void* v )
{
	memory_size -= static_cast<RDORuntimeObject*>(v)->object_size;
	::operator delete( v );
}
#endif

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
RDORuntimeParent::RDORuntimeParent( RDORuntimeParent* _parent ):
	RDORuntimeObject( _parent )
{
}

RDORuntimeParent::~RDORuntimeParent()
{
	deleteObjects();
}

} // namespace rdoRuntime
