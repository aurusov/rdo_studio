/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_BUILD_EDIT_LINE_INFO_H_
#define _RDO_BUILD_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/error_code.h"
#include "simulator/report/rdo_log_edit_line_info.h"
// --------------------------------------------------------------------------------

class RDOBuildEditLineInfo : public RDOLogEditLineInfo
{
public:
	explicit RDOBuildEditLineInfo( CREF(rdo::simulation::report::RDOSyntaxMessage) message );
	explicit RDOBuildEditLineInfo( CREF(tstring) message );
	virtual ~RDOBuildEditLineInfo();

	virtual tstring getMessage() const;
};

#endif // _RDO_BUILD_EDIT_LINE_INFO_H_