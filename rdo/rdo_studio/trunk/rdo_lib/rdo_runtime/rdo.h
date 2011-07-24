/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo.h
 * @authors   Урусов Андрей
 * @date      11.06.2006
 * @brief     Определение интерфейса IBaseOperation
 * @details   Через него осуществляются базовые операции имитации всех сущностей РДО
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_RDO_H_
#define _LIB_RUNTIME_RDO_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/// @todo Надо бы сделать IBaseOperation базовым для всех возможных логик

/******************************************************************************//**
 * @interface IBaseOperation
 * @brief     Интерфейс для паттернов, процессов и блоков процесса
 *********************************************************************************/
class IBaseOperation
{
public:
	/**
	 * @enum  BOResult
	 * @brief Статуc состояния операции
	 */
	enum BOResult
	{
		BOR_cant_run       = 0,
		BOR_planned_and_run,
		BOR_must_continue  ,
		BOR_done
	};

	// Вызывается перед стартом прогона и перед возобновление операции
	// Например, условие DPT-some поменялось с false на true
	// Используется для GENERATE, чтобы задать время прихода первого клиента
	virtual void onStart(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// Вызывается при остановке операции, например, условие DPT-some поменялось с true на false
	virtual void onStop(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// Вызывается для проверки выполнимости операции
	virtual rbool onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// Вызывается для выполнения правил/операций
	virtual BOResult onDoOperation(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// Вызывается для запланированных в будующем событий: event, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void onMakePlaned(CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param) = 0;

	// Вызывается для продолжения долгой операции, например, DPT search
	virtual BOResult onContinue(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IBaseOperation
 * @brief   unknown
 *********************************************************************************/
#define DECLARE_IBaseOperation                                                                   \
	virtual void     onStart         (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onStop          (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual rbool    onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual BOResult onDoOperation   (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onMakePlaned    (CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param); \
	virtual BOResult onContinue      (CREF(rdoRuntime::LPRDORuntime) pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif // _LIB_RUNTIME_RDO_H_
