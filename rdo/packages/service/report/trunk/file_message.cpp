/*!
  \copyright (c) RDO-Team, 2012
  \file      file_message.cpp
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      1.10.2012
  \brief     Файловое сообщение
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/file_message.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

FileMessage::FileMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type)
	: text(text)
	, file(file)
	, line(line)
	, pos (pos )
	, type(type)
{}

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE