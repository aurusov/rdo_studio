#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntimeObject
{
public:
    RDORuntimeObject();
    virtual ~RDORuntimeObject();
};

class RDOSrcInfo
{
public:
        struct Position
    {
        static const std::size_t UNDEFINE_POS  = std::size_t(~0);
        static const std::size_t UNDEFINE_LINE = std::size_t(~0);

        std::size_t m_first_line;
        std::size_t m_first_pos;
        std::size_t m_last_line;
        std::size_t m_last_pos;

        Position();
        Position(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos);
        bool empty() const;
        bool point() const;
    };

    RDOSrcInfo();
    virtual ~RDOSrcInfo();

    void setSrcInfo(const RDOSrcInfo& info);
    void setSrcInfo(const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end);

    void setSrcPos(const Position& position);
    void setSrcPos(const Position& position_begin, const Position& position_end);
    void setSrcPos(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos);

    virtual void setSrcText(const std::string& value);
    void setSrcFileType(rdo::model::FileType value);

    const RDOSrcInfo& src_info() const;
    const Position& src_pos() const;
    const std::string& src_text() const;
    rdo::model::FileType src_filetype() const;
    bool src_empty() const;

private:
    Position m_position;
    std::string m_text_data;
    rdo::model::FileType m_file_type;
};

CLOSE_RDO_RUNTIME_NAMESPACE
