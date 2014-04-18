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
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/common/rdocommon.h"

#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
#include "converter/smr2rdox/grammar/rdobison.h"
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/update/document.h"
#include "converter/smr2rdox/update/update_i.h"

#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

typedef int (*t_bison_parse_fun)(void* lexer);
typedef void (*t_bison_error_fun)(const char* message);
typedef int (*t_flex_lexer_fun)(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
class Converter;

PREDECLARE_POINTER(RDOParserItem);
class RDOParserItem: public rdo::counter_reference
{
DECLARE_FACTORY(RDOParserItem);
public:
	rdo::converter::smr2rdox::RDOFileTypeIn m_type;

	t_bison_parse_fun m_parser_fun;
	t_bison_error_fun m_error_fun;
	t_flex_lexer_fun  m_lexer_fun;

	virtual void parse(Converter* /*pParser*/)
	{}
	virtual void parse(Converter* /*pParser*/, std::istream& /*streamIn*/)
	{}

	virtual std::size_t lexer_loc_line() { return std::size_t(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE); };
	virtual std::size_t lexer_loc_pos() { return 0; };

	bool needStream() const
	{
		return m_needStream;
	}

protected:
	RDOParserItem();
	RDOParserItem(rdo::converter::smr2rdox::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun);
	virtual ~RDOParserItem();

	bool m_needStream;

private:
	void init();
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainer
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParserContainer);
class RDOParserContainer: public rdo::counter_reference
{
DECLARE_FACTORY(RDOParserContainer);
public:
	typedef std::map<std::size_t, LPRDOParserItem> List;
	typedef List::iterator Iterator;

	static const std::size_t UNDEFINED_ID = std::size_t(~0);

	Iterator begin() { return m_list.begin(); }
	Iterator end() { return m_list.end(); }
	Iterator find(std::size_t index) { return m_list.find(index); }

	static void getMinMax(rdo::converter::smr2rdox::RDOParseType type, std::size_t& min, std::size_t& max);

protected:
	RDOParserContainer();
	virtual ~RDOParserContainer();

	std::size_t insert(rdo::converter::smr2rdox::RDOParseType type, const LPRDOParserItem& pParser);

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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_BASE_H_
