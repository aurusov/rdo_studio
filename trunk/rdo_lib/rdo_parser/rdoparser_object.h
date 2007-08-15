#ifndef RDOPARSER_OBJECT_H
#define RDOPARSER_OBJECT_H

#include "rdogramma.h"
#include <rdoruntime_object.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
class RDODeletable
{
private:
	size_t object_size;

public:
	RDODeletable();
	virtual ~RDODeletable();

	void noAutoDelete();

#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif
};

// ----------------------------------------------------------------------------
// ---------- RDOParserObject
// ----------------------------------------------------------------------------
class RDOParser;

class RDOParserObject: public RDODeletable
{
protected:
	RDOParser*             parser;
	const RDOParserObject* parent;
	RDOParserObject( RDOParser* _parser );
	RDOParserObject( const RDOParserObject* _parent );
	virtual ~RDOParserObject();

public:
	void reparent( const RDOParserObject* _parent );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
class RDOParserSrcInfo: public rdoRuntime::RDOSrcInfo
{
private:
	void init();

public:
	RDOParserSrcInfo();
	RDOParserSrcInfo( const YYLTYPE& _pos );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo& _info );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo::Position& _pos );
	RDOParserSrcInfo( const std::string& _text );
	RDOParserSrcInfo( const YYLTYPE& _pos, const std::string& _text );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, const std::string& _text );

	virtual void setSrcInfo( const RDOParserSrcInfo& copy ) {
		RDOSrcInfo::setSrcPos( copy.src_pos() );
		RDOSrcInfo::setSrcText( copy.src_text() );
		RDOSrcInfo::setSrcFileType( copy.src_filetype() );
	}
	virtual void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end ) {
		RDOSrcInfo::setSrcPos( begin.src_pos().first_line, begin.src_pos().first_pos, end.src_pos().last_line, end.src_pos().last_pos );
		RDOSrcInfo::setSrcText( begin.src_text() + delim + end.src_text() );
		RDOSrcInfo::setSrcFileType( begin.src_filetype() );
	}

	virtual void setSrcPos( const YYLTYPE& _pos ) {
		RDOSrcInfo::setSrcPos( _pos.first_line, _pos.first_column, _pos.last_line, _pos.last_column );
	}
	virtual void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end ) {
		RDOSrcInfo::setSrcPos( _pos_begin.first_line, _pos_begin.first_column, _pos_end.last_line, _pos_end.last_column );
	}
	virtual void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		RDOSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	}
	void setSrcPosByLength( const YYLTYPE& _pos, const std::string& _text ) {
		RDOSrcInfo::setSrcPos( _pos.first_line, _pos.first_column, _pos.last_line, _pos.last_column );
		if ( _pos.first_line == _pos.last_line ) {
			rdoRuntime::RDOSrcInfo::Position position = src_pos();
			position.last_pos = position.first_pos + _text.length();
			RDOSrcInfo::setSrcPos( position );
		}
	}
	void setSrcPosAndTextByLength( const YYLTYPE& _pos, const std::string& _text ) {
		setSrcPosByLength( _pos, _text );
		RDOSrcInfo::setSrcText( _text );
	}
	YYLTYPE getPosAsYY() const {
		YYLTYPE pos1;
		rdoRuntime::RDOSrcInfo::Position pos2 = src_pos();
		pos1.first_line   = pos2.first_line;
		pos1.first_column = pos2.first_pos;
		pos1.last_line    = pos2.last_line;
		pos1.last_column  = pos2.last_pos;
		return pos1;
	}
	static int getPosByLength( int _pos, const std::string& _text ) {
		return _pos + _text.length();
	}
};

template <class T> class comparePointers
{
public:
	const T * const val;
	comparePointers(const T * const _val): val(_val) {}
	bool operator() (const T * const other) { return ((*other) == (*val)); }
};

template <class T> class compareName
{
public:
	const std::string * const name;
	compareName(const std::string * const _name): name(_name) {}
	bool operator() (const T * other) { return ((*(other->getName())) == (*name)); }
};

template <class T> class compareName2
{
public:
	const std::string& name;
	compareName2( const std::string& _name ): name(_name) {}
	bool operator() (const T* other) { return other->getName() == name; }
};

} // namespace rdoParse

#endif // RDOPARSER_OBJECT_H
