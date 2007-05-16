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
RDORuntimeObject::RDORuntimeObject( RDORuntimeParent* _parent ):
	parent( _parent )
{
	if ( parent ) {
		parent->insertObject( this );
	} else {
		TRACE( "========= no parent for %d\n", this );
	}
}

RDORuntimeObject::~RDORuntimeObject()
{
	TRACE( "destroy %d\n", this );
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

RDORuntime* RDORuntimeObject::getRuntime()
{
	if ( !parent ) return static_cast<RDORuntime*>(this);
	RDORuntimeObject* _parent = parent;
	while ( _parent->parent ) {
		_parent = _parent->parent;
	}
	return static_cast<RDORuntime*>(_parent);
}

#ifndef _DEBUG
void* RDORuntimeObject::operator new( size_t sz )
{
	RDORuntimeObject* obj = static_cast<RDORuntimeObject*>(::operator new( sz ));
	obj->object_size = sz;
	obj->runtime->memory_insert( sz );
	return obj;
}

void RDORuntimeObject::operator delete( void* v )
{
	static_cast<RDORuntimeObject*>(v)->runtime->memory_remove( static_cast<RDORuntimeObject*>(v)->object_size );
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

void RDORuntimeParent::deleteObjects()
{
	std::list< RDORuntimeObject* >::reverse_iterator it = objects.rbegin();
	while ( it != objects.rend() ) {
		delete *it;
		it = objects.rbegin();
	}
	objects.clear();
}

void RDORuntimeParent::insertObject( RDORuntimeObject* object )
{
	if ( object ) {
		TRACE( "insert object: %d\n", object );
		if ( object == this ) {
			TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
		} else {
			objects.push_back( object );
		}
	} else {
		TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
	}
}

void RDORuntimeParent::removeObject( RDORuntimeObject* object )
{
	std::list< RDORuntimeObject* >::iterator it = std::find( objects.begin(), objects.end(), object );
	if ( it != objects.end() ) {
		TRACE( "remove object: %d\n", object );
		objects.erase( it );
	} else {
		TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
	}
}

} // namespace rdoRuntime
