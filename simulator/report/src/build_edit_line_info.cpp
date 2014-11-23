/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief
  \indent    4T
 */

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/report/src/build_edit_line_info.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

namespace
{
	const std::string WARNING_STRING("предупреждение");
	const std::string ERROR_STRING("ошибка");
}

BuildEditLineInfo::BuildEditLineInfo(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type)
	: LogEditLineInfo(text, file, line, pos, type)
{}

BuildEditLineInfo::BuildEditLineInfo(const FileMessage& message)
	: LogEditLineInfo(message)
{}

BuildEditLineInfo::BuildEditLineInfo(const std::string& message)
	: LogEditLineInfo(message)
{}

BuildEditLineInfo::~BuildEditLineInfo()
{}

std::string BuildEditLineInfo::getMessage() const
{
	if (isSimpleTextMessage())
	{
		return getText();
	}
	else
	{
		const std::string file = rdo::model::getFileTypeString(getFileType());
		const std::string error = (getType() == FileMessage::MT_WARNING) ? WARNING_STRING : ERROR_STRING;
		const std::string text = rdo::format("%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, error.c_str(), getText().c_str());
		return text;
	}
}

}}} // namespace rdo::simulation::report
