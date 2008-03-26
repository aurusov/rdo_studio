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

	rdoModelObjects::RDOFileType type;

	t_bison_parse_fun parser_fun;
	t_bison_error_fun error_fun;
	t_flex_lexer_fun  lexer_fun;

	RDOParserItem( RDOParser* parser ):
		RDODeletable( parser ),
		type( rdoModelObjects::PAT ),
		parser_fun( NULL ),
		error_fun( NULL ),
		lexer_fun( NULL )
	{
	};
	RDOParserItem( RDOParser* parser, rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ):
		RDODeletable( parser ),
		type( _type ),
		parser_fun( _parser_fun ),
		error_fun( _error_fun ),
		lexer_fun( _lexer_fun )
	{
	};
	virtual ~RDOParserItem() {};

	virtual void parse( StreamFrom streamFrom = sf_repository ) = 0;
	virtual void parse( std::istream& in_stream )
	{
		parse();
	}

	virtual int  lexer_loc_line()                   { return -1; };
	virtual int  lexer_loc_pos()                    { return 0;  };
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainer
// ----------------------------------------------------------------------------
class RDOParserContainer: public RDODeletable
{
public:
	typedef std::map< int, RDOParserItem* > List;
	typedef List::const_iterator           CIterator;

	CIterator begin() const           { return m_list.begin();       }
	CIterator end()   const           { return m_list.end();         }
	CIterator find( int index ) const { return m_list.find( index ); }

	static void getMinMax( rdoModelObjects::RDOParseType type, int& min, int& max );
	static std::list< rdoModelObjects::RDOFileType > getFiles( int files );

protected:
	RDOParserContainer( RDOParser* parser );

	int insert( rdoModelObjects::RDOParseType type, RDOParserItem* parser );
	virtual void init() = 0;

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

protected:
	virtual void init();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
class RDOParserContainerSMRInfo: public RDOParserContainer
{
public:
	RDOParserContainerSMRInfo( RDOParser* parser );

protected:
	virtual void init();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerCorba
// ----------------------------------------------------------------------------
class RDOParserContainerCorba: public RDOParserContainer
{
public:
	RDOParserContainerCorba( RDOParser* parser );

protected:
	virtual void init();
};

} // namespace rdoParse

#endif // RDOPARSER_BASE_H
