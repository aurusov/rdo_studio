/*!
  \copyright (c) RDO-Team, 2011
  \file      result.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.01.2007
  \brief     Собираемые при моделированиии показатели
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESULT_H_
#define _LIB_RUNTIME_RESULT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/result/result_i.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/notify_i.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOPMDResult
  \brief     Базовый собираемый показатель
  \todo      Дать классу адекватное англоязычное имя
*/
class RDOPMDResult: public RDOResultTrace, public IName
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOResultTrace)
	QUERY_INTERFACE       (IName         )
QUERY_INTERFACE_END

protected:
	RDOPMDResult(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace);
	virtual ~RDOPMDResult();

	DECLARE_IName;

private:
	tstring m_name;
};

/*!
  \class     RDOPMDWatchPar
  \brief     Собираемый показатель типа WatchPar
*/
class RDOPMDWatchPar: public RDOPMDResult, public IResult, public IModelStructure, public INotify
{
DEFINE_IFACTORY(RDOPMDWatchPar);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDResult     )
	QUERY_INTERFACE       (IResult          )
	QUERY_INTERFACE       (IModelStructure  )
	QUERY_INTERFACE       (IResultTraceValue)
	QUERY_INTERFACE       (INotify          )
QUERY_INTERFACE_END

private:
	RDOPMDWatchPar(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resName, CREF(tstring) parName, ruint resourceID, ruint paramID);
	virtual ~RDOPMDWatchPar();

	LPRDOResource m_pResource;
	ruint         m_resourceID;
	ruint         m_paramID;

	int           m_watchNumber;
	RDOValue      m_currValue;
	double        m_sum;
	RDOValue      m_minValue;
	RDOValue      m_maxValue;

	double        m_timeBegin;
	double        m_timePrev;

	double        m_timeErase;

	DECLARE_INotify;
	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IModelStructure;
};

/*!
  \class     RDOPMDWatchState
  \brief     Собираемый показатель типа WatchState
*/
class RDOPMDWatchState: public RDOPMDResult, public IResult, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchState);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDResult     )
	QUERY_INTERFACE       (IResult          )
	QUERY_INTERFACE       (IModelStructure  )
	QUERY_INTERFACE       (IResultTraceValue)
QUERY_INTERFACE_END

private:
	RDOPMDWatchState(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(LPRDOCalc) pLogic);
	virtual ~RDOPMDWatchState();

	LPRDOCalc m_pLogicCalc;

	int       m_watchNumber;
	rbool     m_currValue;
	double    m_sum;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IModelStructure;
};

/*!
  \class     RDOPMDWatchQuant
  \brief     Собираемый показатель типа WatchQuant
*/
class RDOPMDWatchQuant: public RDOPMDResult, public IResult, public IResultWatchQuant, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchQuant);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDResult     )
	QUERY_INTERFACE       (IResult          )
	QUERY_INTERFACE       (IResultTraceValue)
	QUERY_INTERFACE       (IResultWatchQuant)
	QUERY_INTERFACE       (IModelStructure  )
QUERY_INTERFACE_END

private:
	RDOPMDWatchQuant(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID);
	virtual ~RDOPMDWatchQuant();

	LPRDOCalc m_pLogicCalc;
	int       m_rtpID;

	int       m_watchNumber;
	int       m_currValue;
	double    m_sum;
	double    m_sumSqr;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IResultWatchQuant;
	DECLARE_IModelStructure;
};

/*!
  \class     RDOPMDWatchValue
  \brief     Собираемый показатель типа WatchValue
*/
class RDOPMDWatchValue: public RDOPMDResult, public IResult, public IResultWatchValue, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDResult     )
	QUERY_INTERFACE       (IResult          )
	QUERY_INTERFACE       (IResultTraceValue)
	QUERY_INTERFACE       (IResultWatchValue)
	QUERY_INTERFACE       (IModelStructure  )
QUERY_INTERFACE_END

private:
	RDOPMDWatchValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID);
	virtual ~RDOPMDWatchValue();

	LPRDOCalc m_pLogicCalc;
	LPRDOCalc m_pArithmCalc;
	int       m_rtpID;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IResultWatchValue;
	DECLARE_IModelStructure;
};

/*!
  \class     RDOPMDGetValue
  \brief     Собираемый показатель типа GetValue
*/
class RDOPMDGetValue: public RDOPMDResult, public IResult, public IResultGetValue, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDGetValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDResult     )
	QUERY_INTERFACE       (IResult          )
	QUERY_INTERFACE       (IModelStructure  )
	QUERY_INTERFACE       (IResultTraceValue)
	QUERY_INTERFACE       (IResultGetValue  )
QUERY_INTERFACE_END

private:
	RDOPMDGetValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, CREF(LPRDOCalc) pArithm);
	virtual ~RDOPMDGetValue();

	LPRDOCalc m_pArithmCalc;
	RDOValue  m_value;

	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IResultGetValue;
	DECLARE_IModelStructure;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RESULT_H_
