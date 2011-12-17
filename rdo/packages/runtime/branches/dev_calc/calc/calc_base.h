/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Вычислитель
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_H_
#define _LIB_RUNTIME_CALC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/smart_ptr/factory.h"
#include "utils/smart_ptr/interface_ptr.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_memory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

//! \defgroup calc      Общая группа калков
//! \defgroup calc_base Абстрактный калк

/*!
  \class    RDOCalc
  \brief    Абстрактный "вычислитель", он же калк.
  \details  Cкомпилированная модель состоит из последовательоности калков, так же как и скомпилированная программа состоит из ассемблеровских команд. Данный класс описывает базовый, абстрактный калк.
  \ingroup  calc calc_base
*/
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
//! \details Фабрика для автоматического удаления калка
DECLARE_FACTORY(RDOCalc)

public:
	//! Расчет значение калка. Является публичным методом, через который у калка запрашивается значение
	//! \param pRuntime - указатель на объект runtime'а.
	//!                   Используется для доступа к БД модели, системному времени, генерации ошибок и др.
	//! \result Вычесленное калком значение
	REF(RDOValue) calcValue(CREF(LPRDORuntime) pRuntime);

	//! Сравнение двух калков как объектов (результаты калков в сравнении не используются).
	//! Реализовывать этот метод в новых калках необязательно.
	//! \param pCalc - калк, с которым просводит сравнение текущий
	//! \result true, если калки одинаковые. Значение по умолчанию false
	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	//! Атрибут должен хранить последнее значение калка и возвращать его в \ref doCalc
	RDOValue m_value;

	//! Абстрактное действие калка.
	//! Метод должен быть реализован в потомках (реальных калках).
	//! \param pRuntime - указатель на объект runtime'а.
	//!                   Используется для доступа к БД модели, системному времени, генерации ошибок и др.
	//! \exception RDORuntimeException
	//! \result Вычесленное калком значение
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime) = 0;
};

#define DECLARE_ICalc \
private:              \
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

//! \def    CALC_SUB
//! \brief  Описывает класс-потомок
#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

//! \def    CALC
//! \brief  Описывает класс-потомок от RDOCalc
#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

//! Вспомогательный класс для вытаскивания ресурса по ID и оборачивания его в RDOValue
class RDOCalcGetResourceHelper
{
public:
	static rbool getResource(CREF(LPRDORuntime) pRuntime, ruint resourceID, REF(RDOValue) result);
};

//! Получение ресурса по калку, который возвращает ID
CALC(RDOCalcGetResourceByCalcID)
{
DECLARE_FACTORY(RDOCalcGetResourceByCalcID)
private:
	RDOCalcGetResourceByCalcID(CREF(LPRDOCalc) pGetResourceID);

	LPRDOCalc m_pGetResourceID;

	DECLARE_ICalc;
};

//! Получение ресурса сразу по ID
CALC(RDOCalcGetResourceByID)
{
DECLARE_FACTORY(RDOCalcGetResourceByID)
private:
	RDOCalcGetResourceByID(CREF(ruint) resourceID);

	ruint m_resourceID;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetResID
  \brief   Получение ID ресурса по калку
*/
CALC(RDOCalcGetResID)
{
DECLARE_FACTORY(RDOCalcGetResID)

private:
	LPRDOCalc m_pCalcGetResource;
protected:
	RDOCalcGetResID(CREF(LPRDOCalc) pCalcGetResource);

	DECLARE_ICalc;
};

//! Параметр ресурса по калку ресурса и ID параметра
CALC(RDOCalcGetResParamByCalc)
{
DECLARE_FACTORY(RDOCalcGetResParamByCalc)
private:
	RDOCalcGetResParamByCalc(CREF(LPRDOCalc) pResource, ruint paramID);

	LPRDOCalc m_pResource;
	int       m_paramID;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetUnknowResParam
  \brief   Параметры несуществующего ресурса
*/
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetGroupResParam
  \brief   Получение параметра ресурса из группы
  \todo    Удалить
*/
CALC(RDOCalcGetGroupResParam)
{
DECLARE_FACTORY(RDOCalcGetGroupResParam)
private:
	RDOCalcGetGroupResParam(int parNumb);

	int m_parNumb;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetResourceParamCalc
  \brief   Сохранение калки для ресурса паттерна
*/
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_resNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetPatternParamCalc
  \brief   Сохранение калки для параметра паттерна
*/
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcPatParam
  \brief   Параметр паттерна
*/
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(int numberOfParam);

	int m_numberOfParam;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetTimeNow
  \brief   Получение текущего модельного времени
*/
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetSeconds
  \brief   Получение реального времени работы модели
*/
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetTermNow
  \brief   Получение значения терминального счетчика
*/
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECLARE_ICalc;
};

/*!
  \class   RDOFunCalc
  \brief   Функция
*/
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

/*!
  \class   RDOFuncTableCalc
  \brief   Функция типа таблица
*/
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) res);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc);

	std::vector<LPRDOCalcConst> m_results;
	LPRDOCalc                   m_pArgCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOFunListCalc
  \brief   Функция типа список
*/
CALC_SUB(RDOFunListCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCaseCalc, CREF(LPRDOCalcConst) pResultCalc);

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue);

	std::vector<LPRDOCalc>       m_cases;
	std::vector<LPRDOCalcConst>  m_results;
	LPRDOCalcConst               m_pDefaultValue;

	DECLARE_ICalc;
};

/*!
  \class   RDOFunAlgorithmicCalc
  \brief   Алгоритмическая функция
*/
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct);

protected:
	RDOFunAlgorithmicCalc();

	std::vector<LPRDOCalc> m_conditions;
	std::vector<LPRDOCalc> m_actions;

	DECLARE_ICalc;
};

/*!
  \class   RDOFunAlgorithmicDiapCalc
  \brief   Алгоритмическая функция с проверкой на диапазон возвращаемого значения
*/
CALC_SUB(RDOFunAlgorithmicDiapCalc, RDOFunAlgorithmicCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value);

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECLARE_ICalc;
};

/*!
/*!
/*!

/*!
  \class   RDOCalcFuncParam
  \brief   Параметр функции
*/
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info);

	int m_param_number;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetConst
  \brief   Получение константы
*/
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(int number);

	int m_number;
	DECLARE_ICalc;
};

/*!
  \class   RDOCalcSetConst
  \brief   Установка константы
*/
CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc);

	int        m_number;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcFunctionCall
  \brief   Вызов функции
*/
CALC(RDOCalcFunctionCall)
{
DECLARE_FACTORY(RDOCalcFunctionCall)
public:
	void addParameter(CREF(LPRDOCalc) pCalc);
	void setFunctionCalc(CREF(LPRDOFunCalc) pFunction);

private:
	RDOCalcFunctionCall(CREF(LPRDOFunCalc) pFunction);

	std::vector<LPRDOCalc>  m_parameters;
	LPRDOFunCalc            m_pFunction;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_base.inl"

#endif // _LIB_RUNTIME_CALC_H_
