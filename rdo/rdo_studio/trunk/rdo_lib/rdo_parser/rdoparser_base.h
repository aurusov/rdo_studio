/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_base.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_BASE_H_
#define _RDOPARSER_BASE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdogramma.h"
#include "rdo_lib/rdo_parser/rdoparser_object.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

typedef int  (*t_bison_parse_fun)(PTR(void) lexer  );
typedef void (*t_bison_error_fun)(PTR(char) message);
typedef int  (*t_flex_lexer_fun) (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);

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

	RDOParserItem(PTR(RDOParser) parser )
		: RDODeletable(parser              )
		, m_type      (rdoModelObjects::PAT)
		, m_parser_fun(NULL                )
		, m_error_fun (NULL                )
		, m_lexer_fun (NULL                )
		, m_from      (sf_repository       )
	{}
	RDOParserItem(PTR(RDOParser) parser, rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from = sf_repository)
		: RDODeletable(parser    )
		, m_type      (type      )
		, m_parser_fun(parser_fun)
		, m_error_fun (error_fun )
		, m_lexer_fun (lexer_fun )
		, m_from      (from      )
	{}
	virtual ~RDOParserItem()
	{}

	virtual void  parse() = 0;
	virtual void  parse(REF(std::istream) in_stream) {};

	virtual ruint lexer_loc_line() { return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE; };
	virtual ruint lexer_loc_pos()  { return 0;                                               };

protected:
	StreamFrom m_from;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainer
// ----------------------------------------------------------------------------
class RDOParserContainer: public RDODeletable
{
public:
	typedef std::map<ruint, PTR(RDOParserItem)> List;
	typedef List::const_iterator                CIterator;

	enum {UNDEFINED_ID = ~0};

	CIterator begin() const            { return m_list.begin();     }
	CIterator end  () const            { return m_list.end();       }
	CIterator find (ruint index) const { return m_list.find(index); }

	static void getMinMax(rdoModelObjects::RDOParseType type, REF(ruint) min, REF(ruint) max);

protected:
	RDOParserContainer(PTR(RDOParser) parser);

	ruint insert(rdoModelObjects::RDOParseType type, PTR(RDOParserItem) parser);

private:
	List m_list;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerModel
// ----------------------------------------------------------------------------
class RDOParserContainerModel: public RDOParserContainer
{
public:
	RDOParserContainerModel(PTR(RDOParser) parser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
class RDOParserContainerSMRInfo: public RDOParserContainer
{
public:
	RDOParserContainerSMRInfo(PTR(RDOParser) parser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerCorba
// ----------------------------------------------------------------------------
class RDOParserContainerCorba: public RDOParserContainer
{
public:
	RDOParserContainerCorba(PTR(RDOParser) parser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_BASE_H_
