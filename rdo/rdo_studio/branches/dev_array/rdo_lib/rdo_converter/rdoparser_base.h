/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_base.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_BASE_H_
#define _CONVERTOR_RDOCONVERTER_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/rdocommon.h"
#include "rdo_common/model_objects_convertor.h"

#include "rdo_lib/rdo_converter/rdobison.h"
#include "rdo_lib/rdo_converter/rdogramma.h"
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/update/document.h"
#include "rdo_lib/rdo_converter/update/update_i.h"

#include "rdo_lib/rdo_runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

typedef int  (*t_bison_parse_fun)(PTR(void) lexer  );
typedef void (*t_bison_error_fun)(PTR(char) message);
typedef int  (*t_flex_lexer_fun) (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
class Converter;

OBJECT(RDOParserItem)
{
DECLARE_FACTORY(RDOParserItem);
public:
	rdoModelObjectsConvertor::RDOFileTypeIn m_type;

	t_bison_parse_fun m_parser_fun;
	t_bison_error_fun m_error_fun;
	t_flex_lexer_fun  m_lexer_fun;

	virtual void  parse  (PTR(Converter) pParser)                             {};
	virtual void  parse  (PTR(Converter) pParser, REF(std::istream) streamIn) {};

	virtual ruint lexer_loc_line() { return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE; };
	virtual ruint lexer_loc_pos()  { return 0;                                               };

	rbool needStream() const
	{
		return m_needStream;
	}

protected:
	RDOParserItem();
	RDOParserItem(rdoModelObjectsConvertor::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun);
	virtual ~RDOParserItem();

	rbool m_needStream;

private:
	void init();
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

	enum {UNDEFINED_ID = ~0};

	Iterator begin()            { return m_list.begin();     }
	Iterator end  ()            { return m_list.end();       }
	Iterator find (ruint index) { return m_list.find(index); }

	static void getMinMax(rdoModelObjectsConvertor::RDOParseType type, REF(ruint) min, REF(ruint) max);

protected:
	RDOParserContainer();
	virtual ~RDOParserContainer();

	ruint insert(rdoModelObjectsConvertor::RDOParseType type, CREF(LPRDOParserItem) pParser);

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

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_BASE_H_
