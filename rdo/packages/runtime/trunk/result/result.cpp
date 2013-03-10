/*!
  \copyright (c) RDO-Team, 2011
  \file      result.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.01.2007
  \brief     Собираемые при моделированиии показатели
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#include <boost/format.hpp>

#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/notify.h"
// --------------------------------------------------------------------------------

#if defined( ARCHITECTURE_AMD64 )
typedef ruint64 ruint_type;
#elif defined( ARCHITECTURE_X86 )
typedef ruint ruint_type;
#elif defined( ARCHITECTURE_ARM )
typedef ruint ruint_type;
#endif // ARCHITECTURES_AMD64

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
template <class T>
class ResultStreamItem
{
public:
	ResultStreamItem(rbool predicate, const T& value)
		: predicate(predicate)
		, value    (value    )
	{}

	template <class TS>
	friend rdo::ostream& operator<< (rdo::ostream& stream, const ResultStreamItem<TS>& item);

private:
	rbool predicate;
	T     value;
};

template <class T>
inline rdo::ostream& operator<< (rdo::ostream& stream, const ResultStreamItem<T>& item)
{
	if (item.predicate)
	{
		stream << item.value;
	}
	else
	{
		stream << "нет данных";
	}
	return stream;
}

template <>
inline rdo::ostream& operator<< (rdo::ostream& stream, const ResultStreamItem<double>& item)
{
	if (item.predicate)
	{
		stream << boost::format("%1.6f") % item.value;
	}
	else
	{
		stream << "нет данных";
	}
	return stream;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResult::RDOPMDResult(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace)
	: RDOResultTrace(pRuntime, trace)
	, m_name        (name           )
{
	setTrace(trace);
}

RDOPMDResult::~RDOPMDResult()
{}

CREF(tstring) RDOPMDResult::name() const
{
	return m_name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resName, CREF(tstring) parName, ruint resourceID, ruint paramID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_resourceID  (resourceID           )
	, m_paramID     (paramID              )
	, m_wasFinalCalc(false                )
{
	UNUSED(resName);
	UNUSED(parName);
	pRuntime->notify().connect(this, Notify::RO_BEFOREDELETE);
}

RDOPMDWatchPar::~RDOPMDWatchPar()
{}

void RDOPMDWatchPar::notify(ruint message, PTR(void) pParam)
{
	UNUSED(message);

	if ((ruint_type)pParam == m_resourceID)
	{
		m_resourceID = ruint(~0);
		m_timeErase = m_pRuntime->getCurrentTime();
	}
};

tstring RDOPMDWatchPar::traceValue() const
{
	return rdo::toString(m_currentValue.rdoValue);
}

void RDOPMDWatchPar::resetResult(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(pRuntime);

	m_pResource = pRuntime->getResourceByID(m_resourceID);
	ASSERT(m_pResource);

	m_currentValue = m_pResource->getParam(m_paramID);
	m_timePrev     = m_timeBegin = m_timeErase = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

void RDOPMDWatchPar::checkResult(CREF(LPRDORuntime) pRuntime)
{
	if (!m_pResource)
		return;

	if (m_resourceID == ruint(~0))
		return;

	double currTime = pRuntime->getCurrentTime();
	m_currentValue.weight += currTime - m_timePrev;

	RDOValue newValue = m_pResource->getParam(m_paramID);
	if (newValue != m_currentValue.rdoValue)
	{
		m_acc(m_currentValue.doubleValue, boost::accumulators::weight = m_currentValue.weight);

		m_currentValue = newValue;
		m_wasChanged   = true;
	}

	m_timePrev = currTime;
}

void RDOPMDWatchPar::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	if (!m_wasFinalCalc)
	{
		checkResult(pRuntime);

		m_acc(m_currentValue.doubleValue, boost::accumulators::weight = m_currentValue.weight);

		m_wasFinalCalc = true;
	}

	double average  = boost::accumulators::weighted_mean(m_acc);
	ruint  count    = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::weighted_variance(m_acc);

	rbool averageEnable  = count > 0 && fabs(variance) > DBL_EPSILON;
	rbool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

	double stdDeviation = varianceEnable
		? sqrt(variance)
		: 0.0;

	double cv = averageEnable
		? (variance / average) * 100.0
		: 0.0;

	double median = count > 0
		? boost::accumulators::weighted_median(m_acc)
		: 0.0;

	RDOValue minValue = RDOValue::fromDouble(m_currentValue.rdoValue.type(), (boost::accumulators::min)(m_acc));
	RDOValue maxValue = RDOValue::fromDouble(m_currentValue.rdoValue.type(), (boost::accumulators::max)(m_acc));

	stream.width(30);
	stream << std::left << name()
		<< "\t" << "Тип:"        << "\t" << "par"
		<< "\t" << "Посл.знач.:" << "\t" << ResultStreamItem<tstring> (count > 0, traceValue())
		<< "\t" << "Ср.знач.:"   << "\t" << ResultStreamItem<double>  (count > 0, average     )
		<< "\t" << "Мин.знач.:"  << "\t" << ResultStreamItem<RDOValue>(count > 0, minValue    )
		<< "\t" << "Макс.знач.:" << "\t" << ResultStreamItem<RDOValue>(count > 0, maxValue    )
		<< "\t" << "Числ.наб.:"  << "\t" << count
		<< "\t" << "Стд.откл.:"  << "\t" << ResultStreamItem<double>  (varianceEnable, stdDeviation)
		<< "\t" << "К.вар.%:"    << "\t" << ResultStreamItem<double>  (averageEnable,  cv          )
		<< "\t" << "Медиана:"    << "\t" << ResultStreamItem<double>  (count > 0,      median      )
		<< '\n';
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(LPRDOCalc) pLogic)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (pLogic               )
	, m_wasFinalCalc(false                )
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

tstring RDOPMDWatchState::traceValue() const
{
	return m_currentValue.state ? "TRUE" : "FALSE";
}

void RDOPMDWatchState::resetResult(CREF(LPRDORuntime) pRuntime)
{
	try
	{
		m_currentValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	}
	catch (CREF(RDOUndefinedException))
	{
		m_currentValue = false;
	}
	m_timePrev     = m_timeBegin = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

void RDOPMDWatchState::checkResult(CREF(LPRDORuntime) pRuntime)
{
	rbool newValue;
	try
	{
		newValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	}
	catch(CREF(RDOUndefinedException))
	{
		newValue = false;
	}

	double currTime = pRuntime->getCurrentTime();
	m_currentValue.duration += currTime - m_timePrev;

	if (!m_currentValue.state && newValue) //! from FALSE to TRUE
	{
		m_currentValue = newValue;
		m_wasChanged   = true;
	}
	else if (m_currentValue.state && !newValue) //! from TRUE to FALSE
	{
		m_acc(m_currentValue.duration);
		m_currentValue = newValue;
		m_wasChanged   = true;
	}

	m_timePrev = currTime;
}

void RDOPMDWatchState::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	if (!m_wasFinalCalc)
	{
		if (m_currentValue.state)
		{
			double currTime = pRuntime->getCurrentTime();
			m_currentValue.duration += currTime - m_timePrev;
			m_acc(m_currentValue.duration);
		}

		m_wasFinalCalc = true;
	}

	double currTime = pRuntime->getCurrentTime();
	double average  = boost::accumulators::sum(m_acc) / (currTime - m_timeBegin);
	ruint  count    = boost::accumulators::count(m_acc);

	stream.width(30);
	stream << std::left << name()
		<< "\t" << "Тип:"        << "\t" << "state"
		<< "\t" << "Посл.знач.:" << "\t" << traceValue()
		<< "\t" << "% соотв.:"   << "\t" << boost::format("%1.6f") % average
		<< "\t" << "Мин.длит.:"  << "\t" << boost::format("%1.6f") % (count > 0 ? (boost::accumulators::min)(m_acc) : 0)
		<< "\t" << "Макс.длит.:" << "\t" << boost::format("%1.6f") % (count > 0 ? (boost::accumulators::max)(m_acc) : 0)
		<< "\t" << "Числ.наб.:"  << "\t" << count
		<< '\n';
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (NULL                 )
	, m_rtpID       (rtpID                )
	, m_wasFinalCalc(false                )
{
	UNUSED(resTypeName);
}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

tstring RDOPMDWatchQuant::traceValue() const
{
	return rdo::toString(m_currentValue.quant);
}

void RDOPMDWatchQuant::resetResult(CREF(LPRDORuntime) pRuntime)
{
	m_currentValue = 0;
	m_timePrev     = m_timeBegin = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

ruint RDOPMDWatchQuant::calcCurrentQuant(CREF(LPRDORuntime) pRuntime) const
{
	ruint newQuant = 0;
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != pRuntime->res_end(); ++it)
	{
		if (*it == 0)
			continue;

		if (!(*it)->checkType(m_rtpID))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pLogicCalc->calcValue(pRuntime).getAsBool())
		{
			newQuant++;
		}

		pRuntime->popGroupFunc();
	}
	return newQuant;
}

void RDOPMDWatchQuant::checkResult(CREF(LPRDORuntime) pRuntime)
{
	ruint newQuant = calcCurrentQuant(pRuntime);

	double currTime = pRuntime->getCurrentTime();
	m_currentValue.weight += currTime - m_timePrev;

	if (newQuant != m_currentValue.quant)
	{
		m_acc(m_currentValue.quant, boost::accumulators::weight = m_currentValue.weight);
		m_currentValue = newQuant;
		m_wasChanged   = true;
	}

	m_timePrev = currTime;
}

void RDOPMDWatchQuant::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	if (!m_wasFinalCalc)
	{
		ruint newQuant = calcCurrentQuant(pRuntime);
		if (newQuant != m_currentValue.quant)
		{
			m_acc(m_currentValue.quant, boost::accumulators::weight = m_currentValue.weight);
			m_currentValue = newQuant;
		}
		else
		{
			double currTime = pRuntime->getCurrentTime();
			m_currentValue.weight += currTime - m_timePrev;
		}
		m_acc(m_currentValue.quant, boost::accumulators::weight = m_currentValue.weight);

		m_wasFinalCalc = true;
	}

	double average  = boost::accumulators::weighted_mean(m_acc);
	ruint  count    = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::weighted_variance(m_acc);

	rbool averageEnable  = count > 0 && fabs(variance) > DBL_EPSILON;
	rbool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

	double stdDeviation = varianceEnable
		? sqrt(variance)
		: 0.0;

	double cv = averageEnable
		? (variance / average) * 100.0
		: 0.0;

	double median = count > 0
		? boost::accumulators::weighted_median(m_acc)
		: 0.0;

	stream.width(30);
	stream << std::left << name()
		<< "\t" << "Тип:"        << "\t" << "quant"
		<< "\t" << "Посл.знач.:" << "\t" << ResultStreamItem<tstring>(true, traceValue())
		<< "\t" << "Ср.знач.:"   << "\t" << ResultStreamItem<double> (true, average     )
		<< "\t" << "Мин.знач.:"  << "\t" << ResultStreamItem<int>    (true, (int)(boost::accumulators::min)(m_acc))
		<< "\t" << "Макс.знач.:" << "\t" << ResultStreamItem<int>    (true, (int)(boost::accumulators::max)(m_acc))
		<< "\t" << "Числ.наб.:"  << "\t" << count
		<< "\t" << "Стд.откл.:"  << "\t" << ResultStreamItem<double> (varianceEnable, stdDeviation)
		<< "\t" << "К.вар.%:"    << "\t" << ResultStreamItem<double> (averageEnable,  cv          )
		<< "\t" << "Медиана:"    << "\t" << ResultStreamItem<double> (count > 0,      median      )
		<< '\n';
}

void RDOPMDWatchQuant::setLogicCalc(CREF(LPRDOCalc) pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (NULL                 )
	, m_pArithmCalc (NULL                 )
	, m_rtpID       (rtpID                )
{
	UNUSED(resTypeName);
	m_wasChanged = false;
}

RDOPMDWatchValue::~RDOPMDWatchValue()
{}

tstring RDOPMDWatchValue::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchValue::resetResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	m_currValue = 0;
}

void RDOPMDWatchValue::checkResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPMDWatchValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	UNUSED(pRuntime);

	double average  = boost::accumulators::mean(m_acc);
	ruint  count    = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::variance(m_acc);

	rbool averageEnable  = count > 0 && fabs(variance) > DBL_EPSILON;
	rbool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

	double stdDeviation = varianceEnable
		? sqrt(variance)
		: 0.0;

	double cv = averageEnable
		? (variance / average) * 100.0
		: 0.0;

	double median = count > 0
		? boost::accumulators::median(m_acc)
		: 0.0;

	RDOValue minValue = RDOValue::fromDouble(m_currValue.type(), (boost::accumulators::min)(m_acc));
	RDOValue maxValue = RDOValue::fromDouble(m_currValue.type(), (boost::accumulators::max)(m_acc));

	stream.width(30);
	stream << std::left << name()
		<< "\t" << "Тип:"        << "\t" << "value"
		<< "\t" << "Ср.знач.:"   << "\t" << ResultStreamItem<double>  (count > 0, average )
		<< "\t" << "Мин.знач.:"  << "\t" << ResultStreamItem<RDOValue>(count > 0, minValue)
		<< "\t" << "Макс.знач.:" << "\t" << ResultStreamItem<RDOValue>(count > 0, maxValue)
		<< "\t" << "Числ.наб.:"  << "\t" << count
		<< "\t" << "Стд.откл.:"  << "\t" << ResultStreamItem<double>  (varianceEnable, stdDeviation)
		<< "\t" << "К.вар.%:"    << "\t" << ResultStreamItem<double>  (averageEnable,  cv          )
		<< "\t" << "Медиана:"    << "\t" << ResultStreamItem<double>  (count > 0,      median      )
		<< '\n';
}

void RDOPMDWatchValue::checkResourceErased(CREF(LPRDOResource) pResource)
{
	if (!pResource->checkType(m_rtpID))
	{
		return;
	}
	m_pRuntime->pushGroupFunc(pResource);
	if (m_pLogicCalc->calcValue(m_pRuntime).getAsBool())
	{
		m_currValue  = m_pArithmCalc->calcValue(m_pRuntime);
		m_wasChanged = true;
		traceResult(); /// @todo убрать отсюда ? (и перенести DECLARE_IResultTrace в приват)
//		pRuntime->getTracer()->writeResult(pRuntime, this);
		m_acc(m_currValue.getDouble());
		return;
	}
	m_pRuntime->popGroupFunc();
	return;
}

void RDOPMDWatchValue::setLogicCalc(CREF(LPRDOCalc) pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

void RDOPMDWatchValue::setArithmCalc(CREF(LPRDOCalc) pArithmCalc)
{
	m_pArithmCalc = pArithmCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, CREF(LPRDOCalc) pArithm)
	: RDOPMDResult  (pRuntime, name, false)
	, m_pArithmCalc (pArithm              )
	, m_wasFinalCalc(false                )
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

tstring RDOPMDGetValue::traceValue() const
{
	return "ERROR";
}

void RDOPMDGetValue::resetResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	m_wasFinalCalc = false;
}

void RDOPMDGetValue::checkResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPMDGetValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	if (!m_wasFinalCalc)
	{
		m_value        = m_pArithmCalc->calcValue(pRuntime);
		m_wasFinalCalc = true;
	}

	stream.width(30);
	stream << std::left << name()
		<< "\t" << "Тип:"      << "\t" << "get_value"
		<< "\t" << "Значение:" << "\t" << m_value.getAsString()
		<< '\n';
}

CREF(RDOValue) RDOPMDGetValue::getValue() const
{
	return m_value;
}

void RDOPMDWatchPar::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << " watch_par" << std::endl;
}

void RDOPMDWatchState::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << " watch_state" << std::endl;
}

void RDOPMDWatchQuant::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << " watch_quant" << std::endl;
}

void RDOPMDWatchValue::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << " watch_value" << std::endl;
}

void RDOPMDGetValue::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << " get_value" << std::endl;
}

CLOSE_RDO_RUNTIME_NAMESPACE
