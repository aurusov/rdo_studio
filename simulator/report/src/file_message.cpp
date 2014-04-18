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

FileMessage::FileMessage(const std::string& text, rdoModelObjects::RDOFileType file, std::size_t line, std::size_t pos, Type type)
	: m_text(text)
	, m_file(file)
	, m_line(line)
	, m_pos (pos )
	, m_type(type)
{}

std::string FileMessage::getText() const
{
	return m_text;
}

rdoModelObjects::RDOFileType FileMessage::getFileType() const
{
	return m_file;
}

std::size_t FileMessage::getLineNumber() const
{
	return m_line;
}

std::size_t FileMessage::getPosInLine() const
{
	return m_pos;
}

FileMessage::Type FileMessage::getType() const
{
	return m_type;
}

void FileMessage::setText(const std::string& text)
{
	m_text = text;
}

}}} // namespace rdo::simulation::report
