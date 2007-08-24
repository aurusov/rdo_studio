#include "pch.h"
#include "rdoparser_object.h"
#include "rdoparser.h"
#include <rdo_runtime.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
RDODeletable::RDODeletable()
{
	parser->insertDeletables( this );
}

RDODeletable::~RDODeletable()
{
	noAutoDelete();
}

void RDODeletable::noAutoDelete()
{
	parser->removeDeletables( this );
}

#ifndef _DEBUG
void* RDODeletable::operator new( size_t sz )
{
	RDODeletable* obj = static_cast<RDODeletable*>(::operator new( sz ));
	obj->object_size = sz;
	parser->runtime->memory_insert( sz );
	return obj;
}

void RDODeletable::operator delete( void* v )
{
	parser->runtime->memory_remove( static_cast<RDODeletable*>(v)->object_size );
	::operator delete( v );
}
#endif

// ----------------------------------------------------------------------------
// ---------- RDOParserObject
// ----------------------------------------------------------------------------
RDOParserObject::RDOParserObject( RDOParser* _parser ):
	RDODeletable(),
	parser( _parser ),
	parent( NULL )
{
}

RDOParserObject::RDOParserObject( const RDOParserObject* _parent ):
	RDODeletable(),
	parent( _parent )
{
	parser = _parent->parser;
}

RDOParserObject::~RDOParserObject()
{
}

void RDOParserObject::reparent( const RDOParserObject* _parent )
{
	parent = _parent;
	parser = _parent->parser;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
RDOParserSrcInfo::RDOParserSrcInfo(): RDOSrcInfo()
{
	init();
}

RDOParserSrcInfo::RDOParserSrcInfo( const YYLTYPE& _pos ):
	RDOSrcInfo()
{
	init();
	setSrcPos( _pos );
}

RDOParserSrcInfo::RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo& _info ):
	RDOSrcInfo()
{
	init();
	RDOSrcInfo::setSrcInfo( _info );
}

RDOParserSrcInfo::RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo::Position& _pos ):
	RDOSrcInfo()
{
	init();
	RDOSrcInfo::setSrcPos( _pos );
}

RDOParserSrcInfo::RDOParserSrcInfo( const std::string& _text ):
	RDOSrcInfo()
{
	init();
	setSrcText( _text );
}

RDOParserSrcInfo::RDOParserSrcInfo( const YYLTYPE& _pos, const std::string& _text ):
	RDOSrcInfo()
{
	init();
	setSrcPos( _pos );
	setSrcText( _text );
}

RDOParserSrcInfo::RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end ):
	RDOSrcInfo()
{
	init();
	setSrcPos( _pos_begin, _pos_end );
}

RDOParserSrcInfo::RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, const std::string& _text ):
	RDOSrcInfo()
{
	init();
	setSrcPos( _pos_begin, _pos_end );
	setSrcText( _text );
}

void RDOParserSrcInfo::init()
{
	setSrcFileType( parser->getFileToParse() );
	rdoRuntime::RDOSrcInfo::Position pos = src_pos();
	pos.last_line = parser->lexer_loc_line();
	RDOSrcInfo::setSrcPos( pos );
}

} // namespace rdoParse 
