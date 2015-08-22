// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdoparser.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSrcInfo
// --------------------------------------------------------------------------------
RDOParserSrcInfo::RDOParserSrcInfo()
    : RDOSrcInfo()
{
    init();
}

RDOParserSrcInfo::RDOParserSrcInfo(const YYLTYPE& pos)
    : RDOSrcInfo()
{
    init();
    setSrcPos(pos);
}

RDOParserSrcInfo::RDOParserSrcInfo(const rdo::runtime::RDOSrcInfo& info)
    : RDOSrcInfo()
{
    init();
    RDOSrcInfo::setSrcInfo(info);
}

RDOParserSrcInfo::RDOParserSrcInfo(const rdo::runtime::RDOSrcInfo::Position& pos)
    : RDOSrcInfo()
{
    init();
    RDOSrcInfo::setSrcPos(pos);
}

RDOParserSrcInfo::RDOParserSrcInfo(const std::string& text)
    : RDOSrcInfo()
{
    init();
    setSrcText(text);
}

RDOParserSrcInfo::RDOParserSrcInfo(const YYLTYPE& pos, const std::string& text)
    : RDOSrcInfo()
{
    init();
    setSrcPos (pos );
    setSrcText(text);
}

RDOParserSrcInfo::RDOParserSrcInfo(const YYLTYPE& pos_begin, const YYLTYPE& pos_end, bool first_align)
    : RDOSrcInfo()
{
    init();
    if (!first_align)
    {
        setSrcPos(pos_begin, pos_end);
    }
    else
    {
        if (pos_begin.m_first_line == pos_end.m_last_line)
        {
            setSrcPos(pos_begin, pos_end);
        }
        else
        {
            YYLTYPE pos(pos_begin);
            pos.m_first_line = pos.m_last_line;
            pos.m_first_pos  = pos.m_last_pos;
            setSrcPos(pos);
        }
    }
}

RDOParserSrcInfo::RDOParserSrcInfo(const YYLTYPE& pos_begin, const YYLTYPE& pos_end, const std::string& text)
    : RDOSrcInfo()
{
    init();
    setSrcPos (pos_begin, pos_end);
    setSrcText(text);
}

void RDOParserSrcInfo::init()
{
//    setSrcFileType(Converter::getFileToParse());
    rdo::runtime::RDOSrcInfo::Position pos;
    pos.m_first_pos  = Converter::lexer_loc_pos();
    pos.m_last_pos   = pos.m_first_pos;
    pos.m_first_line = Converter::lexer_loc_line();
    pos.m_last_line  = pos.m_first_line;
    RDOSrcInfo::setSrcPos(pos);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
