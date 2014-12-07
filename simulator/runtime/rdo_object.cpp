// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeObject
// --------------------------------------------------------------------------------
RDORuntimeObject::RDORuntimeObject()
{}

RDORuntimeObject::~RDORuntimeObject()
{}

// --------------------------------------------------------------------------------
// -------------------- Position::Position
// --------------------------------------------------------------------------------
RDOSrcInfo::Position::Position()
	: m_first_line(UNDEFINE_LINE)
	, m_first_pos (UNDEFINE_POS )
	, m_last_line (UNDEFINE_LINE)
	, m_last_pos  (UNDEFINE_POS )
{}

RDOSrcInfo::Position::Position(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos)
	: m_first_line(first_line)
	, m_first_pos (first_pos )
	, m_last_line (last_line )
	, m_last_pos  (last_pos  )
{}

bool RDOSrcInfo::Position::empty() const
{
	return m_first_line == UNDEFINE_LINE && m_first_pos == UNDEFINE_POS && m_last_line == UNDEFINE_LINE && m_last_pos == UNDEFINE_POS;
}

bool RDOSrcInfo::Position::point() const
{
	return m_first_line == m_last_line && m_first_pos == m_last_pos;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSrcInfo
// --------------------------------------------------------------------------------
RDOSrcInfo::RDOSrcInfo()
	: m_text_data(""              )
	, m_file_type(rdo::model::TRC)
{}

RDOSrcInfo::~RDOSrcInfo()
{}

void RDOSrcInfo::setSrcInfo(const RDOSrcInfo& info)
{
	setSrcPos     (info.m_position );
	setSrcText    (info.m_text_data);
	setSrcFileType(info.m_file_type);
}

void RDOSrcInfo::setSrcInfo(const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end)
{
	setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
	setSrcText    (begin.src_text() + delim + end.src_text());
	setSrcFileType(begin.src_filetype());
}

void RDOSrcInfo::setSrcPos(const Position& position)
{
	m_position.m_first_line = position.m_first_line;
	m_position.m_first_pos  = position.m_first_pos;
	m_position.m_last_line  = position.m_last_line;
	m_position.m_last_pos   = position.m_last_pos;
}

void RDOSrcInfo::setSrcPos(const Position& position_begin, const Position& position_end)
{
	m_position.m_first_line = position_begin.m_first_line;
	m_position.m_first_pos  = position_begin.m_first_pos;
	m_position.m_last_line  = position_end.m_last_line;
	m_position.m_last_pos   = position_end.m_last_pos;
}

void RDOSrcInfo::setSrcPos(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos)
{
	m_position.m_first_line = first_line;
	m_position.m_first_pos  = first_pos;
	m_position.m_last_line  = last_line;
	m_position.m_last_pos   = last_pos;
}

void RDOSrcInfo::setSrcText(const std::string& value)
{
	m_text_data = value;
}

void RDOSrcInfo::setSrcFileType(rdo::model::FileType value)
{
	m_file_type = value;
}

const RDOSrcInfo& RDOSrcInfo::src_info() const
{
	return *this;
}

const RDOSrcInfo::Position& RDOSrcInfo::src_pos() const
{
	return m_position;
}

const std::string& RDOSrcInfo::src_text() const
{
	return m_text_data;
}

rdo::model::FileType RDOSrcInfo::src_filetype() const
{
	return m_file_type;
}

bool RDOSrcInfo::src_empty() const
{
	return m_position.empty() && m_text_data.empty() && m_file_type == rdo::model::TRC;
}

CLOSE_RDO_RUNTIME_NAMESPACE
