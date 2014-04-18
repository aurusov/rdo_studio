/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Определение интерфейса IBaseOperation
  \details   Через него осуществляются базовые операции имитации всех сущностей РДО
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RDO_H_
#define _LIB_RUNTIME_RDO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/// @todo Надо бы сделать IBaseOperation базовым для всех возможных логик

/*!
  \interface IBaseOperation
  \brief     Интерфейс для паттернов, процессов и блоков процесса
*/
class IBaseOperation
{
public:
	//! Статуc состояния операции
	enum BOResult
	{
		BOR_cant_run       = 0, //!< операция не может быть запущена
		BOR_planned_and_run,    //!< операция запланирована и выполняется
		BOR_must_continue  ,    //!< операция может быть продолжена
		BOR_done                //!< операция выполнена
	};

	/*!
	  \fn      virtual void onStart(const rdo::runtime::LPRDORuntime& pRuntime)
	  \brief   Запуск
	  \details Вызывается перед стартом прогона и перед возобновление операции
	           Например, условие DPT-some поменялось с false на true
	           Используется для GENERATE, чтобы задать время прихода первого клиента
	  \param   pRuntime константная ссылка на указатель на Рантайм
	*/
	virtual void onStart(const rdo::runtime::LPRDORuntime& pRuntime) = 0;

	/*!
	  \fn      virtual void onStop(const rdo::runtime::LPRDORuntime& pRuntime)
	  \brief   Остановка
	  \details Вызывается при остановке операции, например, условие DPT-some поменялось с true на false
	  \param   pRuntime константная ссылка на указатель на Рантайм
	*/
	virtual void onStop(const rdo::runtime::LPRDORuntime& pRuntime) = 0;

	/*!
	  \fn      virtual bool onCheckCondition(const rdo::runtime::LPRDORuntime& pRuntime)
	  \brief   Проверка
	  \details Вызывается для проверки выполнимости операции
	  \param   pRuntime константная ссылка на указатель на Рантайм
	*/
	virtual bool onCheckCondition(const rdo::runtime::LPRDORuntime& pRuntime) = 0;

	/*!
	  \fn      virtual BOResult onDoOperation(const rdo::runtime::LPRDORuntime& pRuntime)
	  \brief   Выполнение
	  \details Вызывается для выполнения правил/операций
	  \return  BOResult результат выполнения операции
	  \param   pRuntime константная ссылка на указатель на Рантайм
	*/
	virtual BOResult onDoOperation(const rdo::runtime::LPRDORuntime& pRuntime) = 0;

	/*!
	  \fn      virtual BOResult onContinue(const rdo::runtime::LPRDORuntime& pRuntime)
	  \brief   Продолжение
	  \details Вызывается для продолжения долгой операции, например, DPT search
	  \return  BOResult результат выполнения операции
	  \param   pRuntime константная ссылка на указатель на Рантайм
	*/
	virtual BOResult onContinue(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
};

#define DECLARE_IBaseOperation                                                     \
	virtual void     onStart         (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual void     onStop          (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual bool     onCheckCondition(const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual BOResult onDoOperation   (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual BOResult onContinue      (const rdo::runtime::LPRDORuntime& pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);

/*!
  \typedef BaseOperationList
  \brief   Контейнер умных указателей на IBaseOperation
*/
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif // _LIB_RUNTIME_RDO_H_
