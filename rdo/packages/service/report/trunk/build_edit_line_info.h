/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

#ifndef _BUILD_EDIT_LINE_INFO_H_
#define _BUILD_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/error_code.h"
#include "simulator/report/namespace.h"
#include "simulator/report/log_edit_line_info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

class BuildEditLineInfo : public LogEditLineInfo
{
public:
	BuildEditLineInfo( CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type );
	explicit BuildEditLineInfo( CREF(FileMessage) message );
	explicit BuildEditLineInfo( CREF(tstring) message );
	virtual ~BuildEditLineInfo();

	virtual tstring getMessage() const;
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _BUILD_EDIT_LINE_INFO_H_