/*!
  \copyright (c) RDO-Team, 2011
  \file      result_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     Интерфейсы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESULT_I_H_
#define _LIB_RUNTIME_RESULT_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

/*!
  \interface IResult
  \brief     Базовый интерфейс собираемого показателя
*/
class IResult
{
public:
	virtual void resetResult(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void checkResult(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void calcStat   (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream) = 0;
};

#define DECLARE_IResult                                               \
	virtual void resetResult(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void checkResult(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void calcStat   (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream);

/*!
  \interface IResultWatchQuant
  \brief     Интерфейс собираемого показателя WatchQuant
*/
class IResultWatchQuant
{
public:
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc) = 0;
};

#define DECLARE_IResultWatchQuant \
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc);

/*!
  \interface IResultWatchValue
  \brief     Интерфейс собираемого показателя WatchValue
*/
class IResultWatchValue
{
public:
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ) = 0;
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ) = 0;
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc) = 0;
};

#define DECLARE_IResultWatchValue                                                  \
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ); \
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ); \
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc);

/*!
  \interface IResultGetValue
  \brief     Интерфейс собираемого показателя GetValue
*/
class IResultGetValue
{
public:
	virtual CREF(rdoRuntime::RDOValue) getValue() const = 0;
};

#define DECLARE_IResultGetValue \
	CREF(rdoRuntime::RDOValue) getValue() const;

#endif // _LIB_RUNTIME_RESULT_I_H_
