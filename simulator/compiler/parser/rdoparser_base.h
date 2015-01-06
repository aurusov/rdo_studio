#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/common/rdocommon.h"
#include "simulator/compiler/parser/grammar/rdobison.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

typedef int(*t_bison_parse_fun)(void* lexer);
typedef int(*t_flex_lexer_fun)(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

PREDECLARE_POINTER(RDOParserItem);
class RDOParserItem
    : public rdo::counter_reference
    , private boost::noncopyable
{
DECLARE_FACTORY(RDOParserItem);
public:
    enum class StreamFrom
    {
        REPOSITORY,
        EDITOR
    };

    rdo::FileType m_type;

    t_bison_parse_fun m_parser_fun;
    t_flex_lexer_fun  m_lexer_fun;

    virtual void parse(const LPRDOParser& pParser) = 0;
    virtual void parse(const LPRDOParser& pParser, std::istream& in_stream);

    virtual std::size_t lexer_loc_line();
    virtual std::size_t lexer_loc_pos();

protected:
    RDOParserItem();
    RDOParserItem(
        rdo::FileType type,
        t_bison_parse_fun parser_fun,
        t_flex_lexer_fun lexer_fun,
        StreamFrom from = StreamFrom::REPOSITORY
    );
    virtual ~RDOParserItem();

    const StreamFrom m_from;
};

CLOSE_RDO_PARSER_NAMESPACE
