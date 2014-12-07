#ifndef _BUILD_EDIT_LINE_INFO_H_
#define _BUILD_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/error_code.h"
#include "simulator/report/src/log_edit_line_info.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

class BuildEditLineInfo: public LogEditLineInfo
{
public:
	BuildEditLineInfo(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type);
	explicit BuildEditLineInfo(const FileMessage& message);
	explicit BuildEditLineInfo(const std::string& message);
	virtual ~BuildEditLineInfo();

	virtual std::string getMessage() const;
};

}}} // namespace rdo::simulation::report

#endif // _BUILD_EDIT_LINE_INFO_H_
