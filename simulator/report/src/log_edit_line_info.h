/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief
  \indent    4T
 */

#ifndef _LOG_EDIT_LINE_INFO_H_
#define _LOG_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

class LogEditLineInfo : public FileMessage
{
public:
	LogEditLineInfo(CREF(std::string) text, rdoModelObjects::RDOFileType file, std::size_t line, std::size_t pos, Type type);
	explicit LogEditLineInfo(CREF(FileMessage) message);
	explicit LogEditLineInfo(CREF(std::string) message);
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
