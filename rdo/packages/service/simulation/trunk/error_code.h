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

//! Синтаксическая ошибка
struct RDOSyntaxError
{
	//! \brief   Код ошибки
	//! \details Сейчас вместо кода ошибки используется текстовое сообщение об ошибке
	enum ErrorCode
	{
		UNKNOWN = 1, //!< Неизвестная ошибка
	};

	ErrorCode                     m_code;    //!< Код ошибки
	tstring                       m_message; //!< Сообщение об ошибке
	rdoModelObjects::RDOFileType  m_file;    //!< Файл, в котором найдена ошибка
	ruint                         m_line;    //!< Номер строки с ошибкой
	ruint                         m_pos;     //!< Позиция ошибки в строке
	rbool                         m_warning; //!< Признак предупреждения (\b true - предупреждение, \b false - ошибка)

	//! Конструктор ошибки
	//! \param code    - код ошибки
	//! \param message - сообщение об ошибке
	//! \param file    - файл, в котором найдена ошибка
	//! \param line    - номер строки с ошибкой
	//! \param pos     - позиция ошибки в строке
	//! \param warning - признак предупреждения, может отсутствовать. Значение по умолчанию \b false
	RDOSyntaxError(ErrorCode code, CREF(tstring) message, ruint line, ruint pos, rdoModelObjects::RDOFileType file, rbool warning = false)
		: m_code   (code   )
		, m_message(message)
		, m_line   (line   )
		, m_pos    (pos    )
		, m_file   (file   )
		, m_warning(warning)
	{}
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

#endif // _SIMULATOR_SERVICE_ERROR_CODE_H_
