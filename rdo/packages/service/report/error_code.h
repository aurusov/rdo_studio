/*!
  \copyright (c) RDO-Team, 2012
  \file      error_code.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     Коды окончания моделирования
  \indent    4T
*/

#ifndef _SIMULATOR_SERVICE_ERROR_CODE_H_
#define _SIMULATOR_SERVICE_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SERVICE_SIMULATION_NAMESPACE

//! Код окончания компиляции или прогона модели
enum RDOExitCode
{
	EC_OK = 0,        //!< Модель скомпилировалась или завершилась без ошибок
	EC_ParserError,   //!< Компиляция завершилась с ошибкой
	EC_RunTimeError,  //!< Прогон модели завершился с ошибкой
	EC_UserBreak,     //!< Модель остановлена пользователем
	EC_ModelNotFound, //!< Файл прогона модели не найден
	EC_NoMoreEvents   //!< Модель завершилась потому, что больше нечего моделировать
};

struct RDOSyntaxMessage
{
	enum ErrorCode
	{
		UNKNOWN = 1
	};

	enum Type
	{
		MESSAGE_ERROR = 0,
		MESSAGE_WARNING
	};

	tstring                      text;
	ErrorCode                    code;
	rdoModelObjects::RDOFileType file;
	ruint                        line;    //!< Номер строки с ошибкой
	ruint                        pos;     //!< Позиция ошибки в строке
	Type                         type; 

	RDOSyntaxMessage(CREF(tstring) text, ErrorCode code, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MESSAGE_ERROR) : 
		text(text),
		code(code),
		line(line),
		pos (pos ),
		file(file),
		type(type)
	{
	}
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

#endif // _SIMULATOR_SERVICE_ERROR_CODE_H_
