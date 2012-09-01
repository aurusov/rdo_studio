/*!
  \copyright (c) RDO-Team, 2012
  \file      file_message.h
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      1.10.2012
  \brief     Файловое сообщение
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_FILE_MESSAGE_H_
#define _SIMULATOR_REPORT_FILE_MESSAGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
#include "simulator/report/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

//! Файловое сообщение
struct FileMessage
{
	enum Type
	{
		MT_ERROR = 0,
		MT_WARNING
	};

	//! Конструктор сообщения
	//! \param message - текст сообщение
	//! \param file    - файл, в котором найдена ошибка
	//! \param line    - номер строки с ошибкой
	//! \param pos     - позиция ошибки в строке
	//! \param type    - тип сообщения. Значение по умолчанию \b MT_ERROR
	FileMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MT_ERROR);

	tstring                      text; //!< Текст сообщения
	rdoModelObjects::RDOFileType file; //!< Файл с ошибкой
	ruint                        line; //!< Номер строки с ошибкой
	ruint                        pos;  //!< Позиция ошибки в строке
	Type                         type; //!<  Тип сообщения
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_FILE_MESSAGE_H_
