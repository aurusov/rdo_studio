// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/report/src/log_edit_line_info.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

LogEditLineInfo::LogEditLineInfo(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type)
	: FileMessage(text, file, line, pos, type)
	, m_posInLog(0)
	, m_simpleTextMessage(false)
{}

LogEditLineInfo::LogEditLineInfo(const FileMessage& message)
	: FileMessage(message)
	, m_posInLog(0)
	, m_simpleTextMessage(false)
{}

LogEditLineInfo::LogEditLineInfo(const std::string& message)
	: FileMessage(message, rdo::model::PAT, 0, 0)
	, m_posInLog(0)
	, m_simpleTextMessage(true)
{}

LogEditLineInfo::~LogEditLineInfo()
{}

std::string LogEditLineInfo::getMessage() const
{
	if (m_simpleTextMessage)
	{
		return getText();
	}
	else
	{
		const std::string file = rdo::model::getFileTypeString(getFileType());
		const std::string text = rdo::format("%s (%d): %s", file.c_str(), getLineNumber() + 1, getText().c_str());
		return text;
	}
}

bool LogEditLineInfo::isSimpleTextMessage() const
{
	return m_simpleTextMessage;
}

std::size_t LogEditLineInfo::getPosInLog() const
{
	return m_posInLog;
}

void LogEditLineInfo::setPosInLog(int posInLog)
{
	m_posInLog = posInLog;
}

}}} // namespace rdo::simulation::report
