#ifndef _SIMULATOR_REPORT_ERROR_CODE_H_
#define _SIMULATOR_REPORT_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

//! Код окончания компиляции или прогона модели
enum RDOExitCode
{
	EC_OK = 0, //!< Модель скомпилировалась или завершилась без ошибок
	EC_ParserError, //!< Компиляция завершилась с ошибкой
	EC_RunTimeError, //!< Прогон модели завершился с ошибкой
	EC_UserBreak, //!< Модель остановлена пользователем
	EC_ModelNotFound, //!< Файл прогона модели не найден
	EC_NoMoreEvents //!< Модель завершилась потому, что больше нечего моделировать
};

}}} // namespace rdo::simulation::report

#endif // _SIMULATOR_REPORT_ERROR_CODE_H_
