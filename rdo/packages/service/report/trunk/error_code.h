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

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_ERROR_CODE_H_
