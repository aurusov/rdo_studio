¹øòñäãâó ÝçñðþðÝ
#include "pch.h"
#include "rdoparser_object.h"
#include "rdoparser.h"
#include <rdo_runtime.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
RDODeletable::RDODeletable( RDOParser* parser ):
	m_parser( parser )
{
	if ( m_parser )
	{
		m_parser->insertDeletables( this );
		m_parser->runtime()->memory_insert( object_size );
	}
}

RDODeletable::~RDODeletable()
{
	noAutoDelete();
}

void RDODeletable::noAutoDelete()
{
	if ( m_parser )
	{
		m_parser->removeDeletables( this );
	}
}
/*
#ifndef _DEBUG
void* RDODeletable::operator new( size_t sz )
{
	RDODeletable* obj = static_cast<RDODeletable*>(::operator new( sz ));
	obj->object_size = sz;
	return obj;
}

void RDODeletable::operator delete( void* v )
{
	if ( static_cast<RDODeletable*>(v)->m_parser )
	{
		static_cast<RDODeletable*>(v)->m_parser->runtime()->memory_remove( static_cast<RDODeletable*>(v)->object_size );
	}
	::operator delete( v );
}
#endif
*/
// ----------------------------------------------------------------------------
// ---------- RDOParserObject
// ----------------------------------------------------------------------------
RDOParserObject::RDOParserObject( RDOParser* parser ):
	RDODeletable( parser ),
	m_parent( NULL )
{
}

RDOParserObject::RDOParserObject( const RDOParserObject* parent ):
	RDODeletable( parent->m_parser ),
	m_parent( parent )
{
}

RDOParserObject::~RDOParserObject()
{
}

void RDOParserObject::reparent( const RDOParserObject* parent )
{
	m_parent = parent;
	m_parser = parent->m_parser;
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

RDOParserSrcInfo::RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, bool first_align ):
	RDOSrcInfo()
{
	init();
	if ( !first_align )
	{
		setSrcPos( _pos_begin, _pos_end );
	}
	else
	{
		if ( _pos_begin.first_line == _pos_end.last_line )
		{
			setSrcPos( _pos_begin, _pos_end );
		}
		else
		{
			YYLTYPE _pos(_pos_begin);
			_pos.first_line   = _pos.last_line;
			_pos.first_column = _pos.last_column;
			setSrcPos( _pos );
		}
	}
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
	setSrcFileType( RDOParser::getFileToParse() );
	rdoRuntime::RDOSrcInfo::Position pos = src_pos();
	pos.m_last_line = RDOParser::lexer_loc_line();
	RDOSrcInfo::setSrcPos( pos );
}

} // namespace rdoParse 
