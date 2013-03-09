/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_object.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Определение базового класса для всех объектов RDORuntime
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeObject
// --------------------------------------------------------------------------------
inline RDORuntimeObject::RDORuntimeObject()
{}

inline RDORuntimeObject::~RDORuntimeObject()
{}

// --------------------------------------------------------------------------------
// -------------------- Position::Position
// --------------------------------------------------------------------------------
inline RDOSrcInfo::Position::Position()
	: m_first_line(UNDEFINE_LINE)
	, m_first_pos (UNDEFINE_POS )
	, m_last_line (UNDEFINE_LINE)
	, m_last_pos  (UNDEFINE_POS )
{}

inline RDOSrcInfo::Position::Position(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
	: m_first_line(first_line)
	, m_first_pos (first_pos )
	, m_last_line (last_line )
	, m_last_pos  (last_pos  )
{}

inline rbool RDOSrcInfo::Position::empty() const
{
	return m_first_line == UNDEFINE_LINE && m_first_pos == UNDEFINE_POS && m_last_line == UNDEFINE_LINE && m_last_pos == UNDEFINE_POS;
}

inline rbool RDOSrcInfo::Position::point() const
{
	return m_first_line == m_last_line && m_first_pos == m_last_pos;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSrcInfo
// --------------------------------------------------------------------------------
inline RDOSrcInfo::RDOSrcInfo()
	: m_text_data(_T("")              )
	, m_file_type(rdoModelObjects::TRC)
{}

inline RDOSrcInfo::~RDOSrcInfo()
{}

inline void RDOSrcInfo::setSrcInfo(CREF(RDOSrcInfo) info)
{
	setSrcPos     (info.m_position );
	setSrcText    (info.m_text_data);
	setSrcFileType(info.m_file_type);
}

inline void RDOSrcInfo::setSrcInfo(CREF(RDOSrcInfo) begin, CREF(tstring) delim, CREF(RDOSrcInfo) end)
{
	setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
	setSrcText    (begin.src_text() + delim + end.src_text());
	setSrcFileType(begin.src_filetype());
}

inline void RDOSrcInfo::setSrcPos(CREF(Position) position)
{
	m_position.m_first_line = position.m_first_line;
	m_position.m_first_pos  = position.m_first_pos;
	m_position.m_last_line  = position.m_last_line;
	m_position.m_last_pos   = position.m_last_pos;
}

inline void RDOSrcInfo::setSrcPos(CREF(Position) position_begin, CREF(Position) position_end)
{
	m_position.m_first_line = position_begin.m_first_line;
	m_position.m_first_pos  = position_begin.m_first_pos;
	m_position.m_last_line  = position_end.m_last_line;
	m_position.m_last_pos   = position_end.m_last_pos;
}

inline void RDOSrcInfo::setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
{
	m_position.m_first_line = first_line;
	m_position.m_first_pos  = first_pos;
	m_position.m_last_line  = last_line;
	m_position.m_last_pos   = last_pos;
}

inline void RDOSrcInfo::setSrcText(CREF(tstring) value)
{
	m_text_data = value;
}

inline void RDOSrcInfo::setSrcFileType(rdoModelObjects::RDOFileType value)
{
	m_file_type = value;
}

inline CREF(RDOSrcInfo) RDOSrcInfo::src_info() const
{
	return *this;
}

inline CREF(RDOSrcInfo::Position) RDOSrcInfo::src_pos() const
{
	return m_position;
}

inline CREF(tstring) RDOSrcInfo::src_text() const
{
	return m_text_data;
}

inline rdoModelObjects::RDOFileType RDOSrcInfo::src_filetype() const
{
	return m_file_type;
}

inline rbool RDOSrcInfo::src_empty() const
{
	return m_position.empty() && m_text_data.empty() && m_file_type == rdoModelObjects::TRC;
}

CLOSE_RDO_RUNTIME_NAMESPACE
