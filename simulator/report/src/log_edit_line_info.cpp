/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief
  \indent    4T
 */

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/report/src/log_edit_line_info.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

LogEditLineInfo::LogEditLineInfo(const std::string& text, rdoModelObjects::RDOFileType file, std::size_t line, std::size_t pos, Type type)
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
	: FileMessage(message, rdoModelObjects::PAT, 0, 0)
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
		const std::string file = rdoModelObjects::getFileTypeString(getFileType());
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
