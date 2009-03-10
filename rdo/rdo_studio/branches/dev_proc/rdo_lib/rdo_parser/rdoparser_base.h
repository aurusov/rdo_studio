#ifndef RDOPARSER_BASE_H
#define RDOPARSER_BASE_H

#include "rdogramma.h"
#include "rdoparser_object.h"
#include <rdocommon.h>

namespace rdoParse
{

typedef int  (*t_bison_parse_fun)( void* lexer );
typedef void (*t_bison_error_fun)( char* message );
typedef int  (*t_flex_lexer_fun)( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );

// ----------------------------------------------------------------------------
// ---------- RDOParserItem
// ----------------------------------------------------------------------------
class RDOParser;

class RDOParserItem: public RDODeletable
{
public:
	enum StreamFrom
	{
		sf_repository,
		sf_editor
	};

	rdoModelObjects::RDOFileType m_type;

	t_bison_parse_fun m_parser_fun;
	t_bison_error_fun m_error_fun;
	t_flex_lexer_fun  m_lexer_fun;

	RDOParserItem( RDOParser* parser ):
		RDODeletable( parser ),
		m_type( rdoModelObjects::PAT ),
		m_parser_fun( NULL ),
		m_error_fun( NULL ),
		m_lexer_fun( NULL ),
		m_from( sf_repository )
	{
	};
	RDOParserItem( RDOParser* parser, rdoModelObjects::RDOFileType type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository ):
		RDODeletable( parser ),
		m_type( type ),
		m_parser_fun( _parser_fun ),
		m_error_fun( _error_fun ),
		m_lexer_fun( _lexer_fun ),
		m_from( from )
	{
	};
	virtual ~RDOParserItem() {};

	virtual void parse() = 0;
	virtual void parse( std::istream& in_stream ) {};

	virtual int  lexer_loc_line() { return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE; };
	virtual int  lexer_loc_pos()  { return 0;                                               };

protected:
	StreamFrom m_from;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainer
// ----------------------------------------------------------------------------
class RDOParserContainer: public RDODeletable
{
public:
	typedef std::map< int, RDOParserItem* > List;
	typedef List::const_iterator            CIterator;

	CIterator begin() const           { return m_list.begin();       }
	CIterator end()   const           { return m_list.end();         }
	CIterator find( int index ) const { return m_list.find( index ); }

	static void getMinMax( rdoModelObjects::RDOParseType type, int& min, int& max );

protected:
	RDOParserContainer( RDOParser* parser );

	int insert( rdoModelObjects::RDOParseType type, RDOParserItem* parser );

private:
	List m_list;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerModel
// ----------------------------------------------------------------------------
class RDOParserContainerModel: public RDOParserContainer
{
public:
	RDOParserContainerModel( RDOParser* parser );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
class RDOParserContainerSMRInfo: public RDOParserContainer
{
public:
	RDOParserContainerSMRInfo( RDOParser* parser );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerCorba
// ----------------------------------------------------------------------------
class RDOParserContainerCorba: public RDOParserContainer
{
public:
	RDOParserContainerCorba( RDOParser* parser );
};

} // namespace rdoParse

#endif // RDOPARSER_BASE_H
