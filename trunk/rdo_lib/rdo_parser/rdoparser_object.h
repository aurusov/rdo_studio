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
private:
	RDOParser*             parser;
	const RDOParserObject* parent;

protected:
	RDOParserObject( RDOParser* _parser );
	RDOParserObject( const RDOParserObject* _parent );
	virtual ~RDOParserObject();

public:
	RDOParser* getParser() const { return parser; }
	const RDOParserObject* getParent() const { return parent; }
	void reparent( const RDOParserObject* _parent );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
class RDOParserSrcInfo: public rdoRuntime::RDOSrcInfo
{
private:
	void init();
	void setSrcPosAndTextByLength( const YYLTYPE& _pos, const std::string& _text ) {
		RDOSrcInfo::Position pos( _pos.first_line, _pos.first_column, _pos.last_line, _pos.last_column );
		if ( pos.first_line == pos.last_line ) {
			pos.last_pos = pos.first_pos + _text.length();
		}
		setSrcPos( pos );
		setSrcText( _text );
	}

public:
	enum PSI {
		psi_align_none,
		psi_align_bytext
	};
	RDOParserSrcInfo();
	RDOParserSrcInfo( const YYLTYPE& _pos );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo& _info );
	RDOParserSrcInfo( const rdoRuntime::RDOSrcInfo::Position& _pos );
	RDOParserSrcInfo( const std::string& _text );
	RDOParserSrcInfo( const YYLTYPE& _pos, const std::string& _text, PSI psi = psi_align_none );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end );
	RDOParserSrcInfo( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end, const std::string& _text );

	virtual void setSrcInfo( const RDOParserSrcInfo& copy ) {
		setSrcPos( copy.src_pos() );
		setSrcText( copy.src_text() );
		setSrcFileType( copy.src_filetype() );
	}
	 void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end ) {
		RDOParserSrcInfo src_info;
		src_info.setSrcPos( begin.src_pos().first_line, begin.src_pos().first_pos, end.src_pos().last_line, end.src_pos().last_pos );
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
