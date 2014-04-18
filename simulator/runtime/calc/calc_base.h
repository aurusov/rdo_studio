/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_base.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Вычислитель
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_BASE_H_
#define _LIB_RUNTIME_CALC_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_object.h"
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
PREDECLARE_POINTER(RDOCalc);
class RDOCalc: public rdo::counter_reference
{
//! \details Фабрика для автоматического удаления калка
DECLARE_FACTORY(RDOCalc)

public:
	//! Расчет значение калка. Является публичным методом, через который у калка запрашивается значение
	//! \param pRuntime - указатель на объект runtime'а.
	//!                   Используется для доступа к БД модели, системному времени, генерации ошибок и др.
	//! \result Вычесленное калком значение
	RDOValue calcValue(const LPRDORuntime& pRuntime);

	//! Сравнение двух калков как объектов (результаты калков в сравнении не используются).
	//! Реализовывать этот метод в новых калках необязательно.
	//! \param pCalc - калк, с которым проводит сравнение текущий
	//! \result true, если калки одинаковые. Значение по умолчанию false
	virtual bool compare(const LPRDOCalc& pCalc) const;

	//! Узнать привязку калка к исходникам
	const RDOSrcInfo& srcInfo   () const;
	//! Привязать калк к исходникам
	void setSrcInfo(const RDOSrcInfo& srcInfo);

	typedef std::vector<LPRDOCalc> RDOCalcList;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	//! Абстрактное действие калка.
	//! Метод должен быть реализован в потомках (реальных калках).
	//! \param pRuntime - указатель на объект runtime'а.
	//!                   Используется для доступа к БД модели, системному времени, генерации ошибок и др.
	//! \exception RDORuntimeException
	//! \result Вычесленное калком значение
	virtual RDOValue doCalc(const LPRDORuntime& pRuntime) = 0;

private:
	//! Привязка калка к исходникам
	RDOSrcInfo m_srcInfo;
};

#define DECLARE_ICalc \
private:              \
	RDOValue doCalc(const LPRDORuntime& pRuntime);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_base.inl"

#endif // _LIB_RUNTIME_CALC_BASE_H_
