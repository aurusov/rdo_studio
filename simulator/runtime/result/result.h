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
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable: 4510)
	#pragma warning(disable: 4512)
	#pragma warning(disable: 4610)
#endif

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/weighted_mean.hpp>
#include <boost/accumulators/statistics/weighted_variance.hpp>
#include <boost/accumulators/statistics/weighted_median.hpp>

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(default: 4510)
	#pragma warning(default: 4512)
	#pragma warning(default: 4610)
#endif
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
protected:
	RDOPMDResult(const LPRDORuntime& pRuntime, const std::string& name, bool trace);
	virtual ~RDOPMDResult();

	static void printLeft(std::ostream& stream, const std::string& txt);

	DECLARE_IName;

private:
	std::string m_name;
};

/*!
  \class     RDOPMDWatchPar
  \brief     Собираемый показатель типа WatchPar
*/
class RDOPMDWatchPar: public RDOPMDResult, public IResult, public IModelStructure, public INotify
{
DECLARE_FACTORY(RDOPMDWatchPar);
private:
	typedef boost::accumulators::accumulator_set<
		double,
		boost::accumulators::stats<
			boost::accumulators::tag::weighted_mean,
			boost::accumulators::tag::min,
			boost::accumulators::tag::max,
			boost::accumulators::tag::count,
			boost::accumulators::tag::weighted_variance,
			boost::accumulators::tag::weighted_median(boost::accumulators::with_p_square_quantile)
		>,
		double
	> acc_type;

	RDOPMDWatchPar(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& resName, const std::string& parName, const LPRDOCalc& getResourceID, std::size_t paramID);
	virtual ~RDOPMDWatchPar();

	struct CurrentValue
	{
		RDOValue rdoValue;
		double   doubleValue;
		double   weight;

		CurrentValue()
			: doubleValue(0.0)
			, weight     (0.0)
		{}

		CurrentValue(const RDOValue& rdoValue)
			: rdoValue   (rdoValue)
			, doubleValue(rdoValue.getDouble())
			, weight     (0.0)
		{}
	};

	LPRDOResource m_pResource;
	std::size_t m_resourceID;
	std::size_t m_paramID;
	CurrentValue m_currentValue;
	double m_timeBegin;
	double m_timePrev;
	double m_timeErase;
	bool m_wasFinalCalc;
	acc_type m_acc;

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
DECLARE_FACTORY(RDOPMDWatchState);
private:
	typedef boost::accumulators::accumulator_set<
		double,
		boost::accumulators::stats<
			boost::accumulators::tag::sum,
			boost::accumulators::tag::min,
			boost::accumulators::tag::max,
			boost::accumulators::tag::count
		>
	> acc_type;

	RDOPMDWatchState(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const LPRDOCalc& pLogic);
	virtual ~RDOPMDWatchState();

	struct CurrentValue
	{
		bool state;
		double duration;

		CurrentValue()
			: state   (false)
			, duration(0.0  )
		{}

		CurrentValue(bool state)
			: state   (state)
			, duration(0.0  )
		{}
	};

	LPRDOCalc m_pLogicCalc;
	CurrentValue m_currentValue;
	double m_timeBegin;
	double m_timePrev;
	bool m_wasFinalCalc;
	acc_type m_acc;

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
DECLARE_FACTORY(RDOPMDWatchQuant);
private:
	typedef boost::accumulators::accumulator_set<
		double,
		boost::accumulators::stats<
			boost::accumulators::tag::weighted_mean,
			boost::accumulators::tag::min,
			boost::accumulators::tag::max,
			boost::accumulators::tag::count,
			boost::accumulators::tag::weighted_variance,
			boost::accumulators::tag::weighted_median(boost::accumulators::with_p_square_quantile)
		>,
		double
	> acc_type;

	RDOPMDWatchQuant(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& resTypeName, int rtpID);
	virtual ~RDOPMDWatchQuant();

	struct CurrentValue
	{
		std::size_t quant;
		double  weight;

		CurrentValue()
			: quant (0  )
			, weight(0.0)
		{}

		CurrentValue(std::size_t quant)
			: quant (quant)
			, weight(0.0  )
		{}
	};

	LPRDOCalc m_pLogicCalc;
	int m_rtpID;
	CurrentValue m_currentValue;
	double m_timeBegin;
	double m_timePrev;
	bool m_wasFinalCalc;
	acc_type m_acc;

	std::size_t calcCurrentQuant(const LPRDORuntime& pRuntime) const;

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
DECLARE_FACTORY(RDOPMDWatchValue);
private:
	typedef boost::accumulators::accumulator_set<
		double,
		boost::accumulators::stats<
			boost::accumulators::tag::mean,
			boost::accumulators::tag::min,
			boost::accumulators::tag::max,
			boost::accumulators::tag::count,
			boost::accumulators::tag::variance,
			boost::accumulators::tag::median(boost::accumulators::with_p_square_quantile)
		>
	> acc_type;

	RDOPMDWatchValue(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& resTypeName, int rtpID);
	virtual ~RDOPMDWatchValue();

	LPRDOCalc m_pLogicCalc;
	LPRDOCalc m_pArithmCalc;
	int       m_rtpID;
	RDOValue  m_currValue;
	acc_type  m_acc;

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
DECLARE_FACTORY(RDOPMDGetValue);
private:
	RDOPMDGetValue(const LPRDORuntime& pRuntime, const std::string& name, const LPRDOCalc& pArithm);
	virtual ~RDOPMDGetValue();

	LPRDOCalc m_pArithmCalc;
	RDOValue m_value;
	bool m_wasFinalCalc;

	DECLARE_IResult;
	DECLARE_IResultTraceValue;
	DECLARE_IResultGetValue;
	DECLARE_IModelStructure;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RESULT_H_
