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
// ---------- RDOParseErrorPos
// ----------------------------------------------------------------------------
class RDOParseErrorPos: public rdoRuntime::RDOErrorPos
{
public:
	RDOParseErrorPos(): RDOErrorPos() {
	}
	RDOParseErrorPos( const YYLTYPE& _error_pos ): RDOErrorPos() {
		error_pos.first_line   = _error_pos.first_line;
		error_pos.first_column = _error_pos.first_column;
		error_pos.last_line    = _error_pos.last_line;
		error_pos.last_column  = _error_pos.last_column;
	}
	RDOParseErrorPos( const rdoRuntime::RDOErrorPos::Data& _error_pos ): RDOErrorPos() {
		RDOErrorPos::setErrorPos( _error_pos );
	}
	void setErrorPos( const RDOParseErrorPos& copy ) {
		RDOErrorPos::setErrorPos( copy.error_pos );
	}
	void setErrorPos( int first_line, int first_column, int last_line, int last_column ) {
		RDOErrorPos::setErrorPos( first_line, first_column, last_line, last_column );
	}
	YYLTYPE errorYY() const {
		YYLTYPE _error_pos;
		_error_pos.first_line   = error_pos.first_line;
		_error_pos.first_column = error_pos.first_column;
		_error_pos.last_line    = error_pos.last_line;
		_error_pos.last_column  = error_pos.last_column;
		return _error_pos;
	}
};

/*
class RDOErrorPos
{
private:
	YYLTYPE error_pos;

public:
	RDOErrorPos() {
		error_pos.first_line   = -1;
		error_pos.first_column = -1;
		error_pos.last_line    = -1;
		error_pos.last_column  = -1;
	}
	void setErrorPos( const YYLTYPE& _error_pos ) {
		error_pos.first_line   = _error_pos.first_line;
		error_pos.first_column = _error_pos.first_column;
		error_pos.last_line    = _error_pos.last_line;
		error_pos.last_column  = _error_pos.last_column;
	}
	void setErrorPos( int first_line, int first_column, int last_line, int last_column ) {
		error_pos.first_line   = first_line;
		error_pos.first_column = first_column;
		error_pos.last_line    = last_line;
		error_pos.last_column  = last_column;
	}
	const YYLTYPE& error() const { return error_pos; }
};
*/
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
