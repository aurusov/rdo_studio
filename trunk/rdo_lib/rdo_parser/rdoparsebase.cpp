#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparsebase.h"
#include "rdoparser.h"

namespace rdoParse 
{

RDODeletable::RDODeletable()
{
	parser->insertDeletables( this );
}

RDODeletable::~RDODeletable()
{
	parser->removeDeletables( this );
}

#ifndef _DEBUG
void* RDODeletable::operator new( size_t sz )
{
	RDODeletable* obj = static_cast<RDODeletable*>(::operator new( sz ));
	obj->object_size = sz;
	parser->runTime->memory_insert( sz );
	return obj;
}

void RDODeletable::operator delete( void* v )
{
	parser->runTime->memory_remove( static_cast<RDODeletable*>(v)->object_size );
	::operator delete( v );
}
#endif

RDOParserObject::RDOParserObject( RDOParser* _parser ):
	RDODeletable(),
	parser( _parser ),
	parent( NULL )
{
//	parser->insertDeletables( this );
}

RDOParserObject::RDOParserObject( const RDOParserObject* _parent ):
	RDODeletable(),
	parent( _parent )
{
	parser = _parent->parser;
//	parser->insertDeletables( this );
}

RDOParserObject::~RDOParserObject()
{
//	parser->removeDeletables( this );
}

} // namespace rdoParse 
