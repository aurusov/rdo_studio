/*!
  \copyright (c) RDO-Team, 2012
  \file      file_message.cpp
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      1.10.2012
  \brief     Файловое сообщение
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

FileMessage::FileMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type)
	: m_text(text)
	, m_file(file)
	, m_line(line)
	, m_pos (pos )
	, m_type(type)
{}

tstring FileMessage::getText() const
{
	return m_text;
}

rdoModelObjects::RDOFileType FileMessage::getFileType() const
{
	return m_file;
}

ruint FileMessage::getLineNumber() const
{
	return m_line;
}

ruint FileMessage::getPosInLine() const
{
	return m_pos;
}

FileMessage::Type FileMessage::getType() const
{
	return m_type;
}

void FileMessage::setText(CREF(tstring) text)
{
	m_text = text;
}

}}} // namespace rdo::simulation::report
