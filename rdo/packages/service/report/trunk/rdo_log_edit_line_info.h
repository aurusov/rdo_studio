/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_LOG_EDIT_LINE_INFO_H_
#define _RDO_LOG_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "simulator/report/error_code.h"
// --------------------------------------------------------------------------------

class RDOLogEditLineInfo
{
public:
	explicit RDOLogEditLineInfo( CREF(rdo::simulation::report::RDOSyntaxMessage) message );
	explicit RDOLogEditLineInfo( CREF(tstring) text );
	virtual ~RDOLogEditLineInfo();

	virtual tstring getMessage() const;
	rbool isSimpleTextMessage() const;

	rdo::simulation::report::RDOSyntaxMessage::Type getMessageType() const;
	rdoModelObjects::RDOFileType getFileType() const;
	rdo::simulation::report::RDOSyntaxMessage::ErrorCode getErrorCode() const;
	int getLineNumber() const;
	int getPosInLine() const;
	int getPosInLog() const;
	tstring getText() const;

	void setPosInLog(int posInLog);

private:
	rdo::simulation::report::RDOSyntaxMessage m_message;
	rsint m_posInLog;
	rbool m_simpleTextMessage;
};

#endif // _RDO_LOG_EDIT_LINE_INFO_H_