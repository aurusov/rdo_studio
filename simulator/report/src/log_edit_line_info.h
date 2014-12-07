#ifndef _LOG_EDIT_LINE_INFO_H_
#define _LOG_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

class LogEditLineInfo : public FileMessage
{
public:
	LogEditLineInfo(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type);
	explicit LogEditLineInfo(const FileMessage& message);
	explicit LogEditLineInfo(const std::string& message);
	virtual ~LogEditLineInfo();

	virtual std::string getMessage() const;
	bool isSimpleTextMessage() const;

	std::size_t getPosInLog() const;
	void setPosInLog(int posInLog);

private:
	std::size_t m_posInLog;
	bool m_simpleTextMessage;
};

}}} // namespace rdo::simulation::report

#endif // _LOG_EDIT_LINE_INFO_H_
