#include "pch.h"
#include "rdoruntime_object.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
size_t RDORuntimeObject::s_memory_size = 0;

RDORuntimeObject::RDORuntimeObject( RDORuntimeParent* parent ):
	m_parent( parent )
{
	if ( m_parent ) {
		m_parent->insertObject( this );
	} else {
//		TRACE( "========= no parent for %d\n", this );
	}
}

RDORuntimeObject::~RDORuntimeObject()
{
//	TRACE( "destroy %d\n", this );
	if ( m_parent ) {
		m_parent->removeObject( this );
	}
}

void RDORuntimeObject::detach()
{
	if ( m_parent ) {
		m_parent->removeObject( this );
		m_parent = NULL;
	}
}

void RDORuntimeObject::reparent( RDORuntimeParent* parent )
{
	if ( m_parent != parent ) {
		if ( m_parent ) {
			m_parent->removeObject( this );
		}
		m_parent = parent;
		if ( m_parent ) {
			m_parent->insertObject( this );
		}
	}
}
/*
#ifndef _DEBUG
void* RDORuntimeObject::operator new( size_t sz )
{
	s_memory_size += sz;
	RDORuntimeObject* obj = static_cast<RDORuntimeObject*>(::operator new( sz ));
	obj->m_object_size = sz;
//	obj->runtime->memory_insert( sz );
	return obj;
}

void RDORuntimeObject::operator delete( void* v )
{
	s_memory_size -= static_cast<RDORuntimeObject*>(v)->m_object_size;
	::operator delete( v );
}
#endif
*/
// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
RDORuntimeParent::RDORuntimeParent( RDORuntimeParent* parent ):
	RDORuntimeObject( parent )
{
}

RDORuntimeParent::~RDORuntimeParent()
{
	deleteObjects();
}

} // namespace rdoRuntime
