// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

FileMessage::FileMessage(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type)
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

rdo::model::FileType FileMessage::getFileType() const
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
