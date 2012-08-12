/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      15.02.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/report/code.h"
#include "simulator/report/rdo_build_edit_line_info.h"
// --------------------------------------------------------------------------------

RDOBuildEditLineInfo::RDOBuildEditLineInfo( CREF(RDOSyntaxMessage) error ) :
	RDOLogEditLineInfo( error )
{
}

RDOBuildEditLineInfo::RDOBuildEditLineInfo( CREF(tstring) text ) :
	RDOLogEditLineInfo( text )
{
}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{
}

tstring RDOBuildEditLineInfo::getMessage() const
{
	if ( isSimpleTextMessage() )
	{
		return getText();
	}
	else
	{
		tstring file = rdoModelObjects::getFileTypeString(getFileType());
		tstring s_error = (getMessageType() == RDOSyntaxMessage::MT_WARNING) ? rdo::format( IDS_WARNING, getErrorCode() ) : rdo::format( IDS_ERROR, getErrorCode() );
		return rdo::format( "%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, s_error.c_str(), getText().c_str() );
	}
}