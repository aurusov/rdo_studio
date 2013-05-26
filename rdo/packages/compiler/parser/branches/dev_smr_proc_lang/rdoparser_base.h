/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_base.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_BASE_H_
#define _RDOPARSER_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "utils/rdocommon.h"
#include "simulator/compiler/parser/grammar/rdobison.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

typedef int  (*t_bison_parse_fun)(PTR(void) lexer  );
typedef void (*t_bison_error_fun)(PTR(char) message);
typedef int  (*t_flex_lexer_fun) (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

OBJECT(RDOParserItem)
{
DECLARE_FACTORY(RDOParserItem);
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

	virtual void parse(CREF(LPRDOParser) pParser) = 0;
    virtual void parse(CREF(LPRDOParser) pParser, REF(std::istream) in_stream);

    virtual ruint lexer_loc_line();
    virtual ruint lexer_loc_pos ();

protected:
    RDOParserItem();
    RDOParserItem(rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from = sf_repository);
    virtual ~RDOParserItem();

	StreamFrom m_from;
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainer
// --------------------------------------------------------------------------------
OBJECT(RDOParserContainer)
{
DECLARE_FACTORY(RDOParserContainer);
public:
	typedef std::map<ruint, LPRDOParserItem> List;
	typedef List::iterator                   Iterator;

	static const ruint UNDEFINED_ID = ruint(~0);

    Iterator begin();
    Iterator end  ();
    Iterator find (ruint index);
	void     clear();

	static void getMinMax(rdoModelObjects::RDOParseType type, REF(ruint) min, REF(ruint) max);

protected:
	RDOParserContainer();
	virtual ~RDOParserContainer();

	ruint insert(rdoModelObjects::RDOParseType type, CREF(LPRDOParserItem) pParser);

private:
	List m_list;
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerModel
// --------------------------------------------------------------------------------
class RDOParserContainerModel: public RDOParserContainer
{
DECLARE_FACTORY(RDOParserContainerModel);
private:
	RDOParserContainerModel();
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerSMRInfo
// --------------------------------------------------------------------------------
class RDOParserContainerSMRInfo: public RDOParserContainer
{
DECLARE_FACTORY(RDOParserContainerSMRInfo);
private:
	RDOParserContainerSMRInfo();
};

#ifdef CORBA_ENABLE
// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerCorba
// --------------------------------------------------------------------------------
class RDOParserContainerCorba: public RDOParserContainer
{
DECLARE_FACTORY(RDOParserContainerCorba);
private:
	RDOParserContainerCorba();
};
#endif

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_BASE_H_
