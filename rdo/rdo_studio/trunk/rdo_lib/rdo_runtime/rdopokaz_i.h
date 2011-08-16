/*!
  \copyright (c) RDO-Team, 2009
  \file      rdopokaz_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     Интерфейсы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_POKAZ_I_H_
#define _LIB_RUNTIME_POKAZ_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_simulator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// --------------------------------------------------------------------------------

/*!
  \interface IPokaz
  \brief     Базовый интерфейс собираемого показателя
*/
class IPokaz
{
public:
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream) = 0;
};

/*!
  \def     DECLARE_IPokaz
  \brief   Макрос для декларации методов собираемых показателей
*/
#define DECLARE_IPokaz                                               \
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream);

/*!
  \interface IPokazWatchQuant
  \brief     Интерфейс собираемого показателя WatchQuant
*/
class IPokazWatchQuant
{
public:
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc) = 0;
};

/*!
  \def     DECLARE_IPokazWatchQuant
  \brief   Макрос для декларации интерфейса IPokazWatchQuant
*/
#define DECLARE_IPokazWatchQuant \
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc);

/*!
  \interface IPokazWatchValue
  \brief     Интерфейс собираемого показателя WatchValue
*/
class IPokazWatchValue
{
public:
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ) = 0;
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ) = 0;
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc) = 0;
};

/*!
  \def     DECLARE_IPokazWatchValue
  \brief   Макрос для декларации интерфейса IPokazWatchValue
*/
#define DECLARE_IPokazWatchValue                                                   \
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ); \
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ); \
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc);

/*!
  \interface IPokazGetValue
  \brief     Интерфейс собираемого показателя GetValue
*/
class IPokazGetValue
{
public:
	virtual CREF(rdoRuntime::RDOValue) getValue() const = 0;
};

/*!
  \def     DECLARE_IPokazGetValue
  \brief   Макрос для декларации интерфейса IPokazGetValue
*/
#define DECLARE_IPokazGetValue \
	CREF(rdoRuntime::RDOValue) getValue() const;

#endif // _LIB_RUNTIME_POKAZ_I_H_
