/*!
  \copyright (c) RDO-Team, 2012
  \file      error_code.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      16.02.2012
  \brief     Коды окончания моделирования
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_ERROR_CODE_H_
#define _SIMULATOR_REPORT_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
#include "simulator/report/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

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

//! Синтаксическое сообщение
struct RDOSyntaxMessage
{
	enum Type
	{
		MT_ERROR = 0,
		MT_WARNING
	};

	tstring                      text; //!< Текст сообщения
	rdoModelObjects::RDOFileType file; //!< Файл с ошибкой
	ruint                        line; //!< Номер строки с ошибкой
	ruint                        pos;  //!< Позиция ошибки в строке
	Type                         type; //!<  Тип сообщения

	//! Конструктор сообщения
	//! \param message - текст сообщение
	//! \param file    - файл, в котором найдена ошибка
	//! \param line    - номер строки с ошибкой
	//! \param pos     - позиция ошибки в строке
	//! \param type    - тип сообщения. Значение по умолчанию \b MT_ERROR
	RDOSyntaxMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MT_ERROR)
		: text(text)
		, file(file)
		, line(line)
		, pos (pos )
		, type(type)
	{}
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_ERROR_CODE_H_
