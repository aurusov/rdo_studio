#ifndef RDOPARSER_OBJECT_H
#define RDOPARSER_OBJECT_H

#include "rdogramma.h"
#include <rdoruntime_object.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
class RDOParser;

class RDODeletable
{
public:
	RDODeletable( RDOParser* parser );
	virtual ~RDODeletable();

	RDOParser* parser() const { return m_parser; }

	void noAutoDelete();

#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif

protected:
	RDOParser* m_parser;

private:
	size_t     object_size;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserObject
// ----------------------------------------------------------------------------
class RDOParser;

class RDOParserObject: public RDODeletable
{
public:
	const RDOParserObject* parent() const { return m_parent; }
	void reparent( const RDOParserObject* parent );
	virtual ~RDOParserObject();

protected:
	RDOParserObject( RDOParser* parser );
	RDOParserObject( const RDOParserObject* parent );

private:
	const RDOParserObject* m_parent;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
class RDOParserSrcInfo: public rdoRuntime::RDOSrcInfo
{
public:
	RDOParserSrcInfo();
	RDOParserSrcInfo( const YYLTYPE& _pos );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo& _info );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo::Position& _pos );
	RDOParserSrcInfo( const std::string& _text );
	RDOParserSrcInfo( const YYLTYPE& _pos, const std::string& _text );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, bool first_align = false );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, const std::string& _text );

	virtual void setSrcInfo( const RDOParserSrcInfo& copy ) {
		setSrcPos( copy.src_pos() );
		setSrcText( copy.src_text() );
		setSrcFileType( copy.src_filetype() );
	}
	void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end ) {
		RDOParserSrcInfo src_info;
		src_info.setSrcPos( begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos );
		src_info.setSrcText( begin.src_text() + delim + end.src_text() );
		src_info.setSrcFileType( begin.src_filetype() );
		setSrcInfo( src_info );
	}

	virtual void setSrcPos( const RDOSrcInfo::Position& _pos ) {
		RDOSrcInfo::setSrcPos( _pos );
	}
	void setSrcPos( const YYLTYPE& _pos ) {
		RDOSrcInfo::Position pos( _pos.first_line, _pos.first_column, _pos.last_line, _pos.last_column );
		setSrcPos( pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end ) {
		RDOSrcInfo::Position pos ( _pos_begin.first_line, _pos_begin.first_column, _pos_end.last_line, _pos_end.last_column );
		setSrcPos( pos );
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		RDOSrcInfo::Position pos( first_line, first_pos, last_line, last_pos );
		setSrcPos( pos );
	}
	YYLTYPE getPosAsYY() const {
		YYLTYPE pos1;
		rdoRuntime::RDOSrcInfo::Position pos2 = src_pos();
		pos1.first_line   = pos2.m_first_line;
		pos1.first_column = pos2.m_first_pos;
		pos1.last_line    = pos2.m_last_line;
		pos1.last_column  = pos2.m_last_pos;
		return pos1;
	}
	static int getPosByLength( int _pos, const std::string& _text ) {
		return _pos + _text.length();
	}

private:
	void init();
};

template <class T> class compareName
{
public:
	compareName( const std::string& name ): m_name(name) {}
	bool operator() (const T* obj) { return obj->name() == m_name; }

private:
	const std::string& m_name;
};

template <class T> class compareNameRef
{
public:
	compareNameRef( const std::string& name ): m_name(name) {}
	bool operator() (const T& obj) { return obj.name() == m_name; }

private:
	const std::string& m_name;
};

} // namespace rdoParse

#endif // RDOPARSER_OBJECT_H
