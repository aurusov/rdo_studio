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
#include "simulator/report/comment.h"
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
		rsint code = getErrorCode();
		tstring error = (getMessageType() == RDOSyntaxMessage::MT_WARNING) ? rdo::format( WARNING_STRING, code ) : rdo::format( ERROR_STRING, code );
		tstring text = rdo::format( "%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, error.c_str(), getText().c_str() );
		return text;
	}
}