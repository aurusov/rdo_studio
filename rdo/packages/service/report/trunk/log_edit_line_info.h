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
#include "simulator/report/namespace.h"
#include "simulator/report/file_message.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

class LogEditLineInfo : public FileMessage
{
public:
	LogEditLineInfo( CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type );
	explicit LogEditLineInfo( CREF(FileMessage) message );
	explicit LogEditLineInfo( CREF(tstring) message );
	virtual ~LogEditLineInfo();

	virtual tstring getMessage() const;
	rbool isSimpleTextMessage() const;

	ruint getPosInLog() const;
	void setPosInLog(int posInLog);

private:
	ruint m_posInLog;
	rbool m_simpleTextMessage;
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _LOG_EDIT_LINE_INFO_H_