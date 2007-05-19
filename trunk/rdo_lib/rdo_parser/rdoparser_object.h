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
	RDOParserSrcInfo( const YYLTYPE& _error_pos );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo& _info );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo::Position& _pos );

	virtual void setSrcInfo( const RDOParserSrcInfo& copy ) {
		RDOSrcInfo::setSrcPos( copy.src_pos() );
		RDOSrcInfo::setSrcText( copy.src_text() );
		RDOSrcInfo::setSrcFileType( copy.src_filetype() );
	}
	virtual void setSrcInfo( const RDOParserSrcInfo& begin, const RDOParserSrcInfo& end ) {
		RDOSrcInfo::setSrcPos( begin.src_pos().first_line, begin.src_pos().first_pos, end.src_pos().last_line, end.src_pos().last_pos );
		RDOSrcInfo::setSrcText( begin.src_text() + " " + end.src_text() );
		RDOSrcInfo::setSrcFileType( begin.src_filetype() );
	}

	virtual void setSrcPos( const YYLTYPE& _error_pos ) {
		RDOSrcInfo::setSrcPos( _error_pos.first_line, _error_pos.first_column, _error_pos.last_line, _error_pos.last_column );
	}
	virtual void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		RDOSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
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

} // namespace rdoParse

#endif // RDOPARSER_OBJECT_H
