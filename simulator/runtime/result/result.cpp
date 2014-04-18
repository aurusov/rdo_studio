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
#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#include <boost/format.hpp>

#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/notify.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

#if defined( ARCHITECTURE_AMD64 )
typedef uint64_t ruint_type;
#elif defined( ARCHITECTURE_X86 )
typedef std::size_t ruint_type;
#elif defined( ARCHITECTURE_ARM )
typedef std::size_t ruint_type;
#endif // ARCHITECTURES_AMD64

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
template <class T>
class ResultStreamItem
{
public:
	ResultStreamItem(bool predicate, const T& value)
		: predicate(predicate)
		, value    (value    )
	{}

	template <class TS>
	friend std::ostream& operator<< (std::ostream& stream, const ResultStreamItem<TS>& item);

private:
	bool predicate;
	T value;
};

template <class T>
inline std::ostream& operator<< (std::ostream& stream, const ResultStreamItem<T>& item)
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
inline std::ostream& operator<< (std::ostream& stream, const ResultStreamItem<double>& item)
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
RDOPMDResult::RDOPMDResult(const LPRDORuntime& pRuntime, const std::string& name, bool trace)
	: RDOResultTrace(pRuntime, trace)
	, m_name        (name           )
{
	setTrace(trace);
}

RDOPMDResult::~RDOPMDResult()
{}

const std::string& RDOPMDResult::name() const
{
	return m_name;
}

void RDOPMDResult::printLeft(std::ostream& stream, const std::string& txt)
{
	stream << txt;

	int spaceCnt = 30 - (int)rdo::locale::length(txt);
	if (spaceCnt > 0)
	{
		stream.width(spaceCnt);
		stream << " ";
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& /*resName*/, const std::string& /*parName*/, std::size_t resourceID, std::size_t paramID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_resourceID  (resourceID           )
	, m_paramID     (paramID              )
	, m_wasFinalCalc(false                )
{
	pRuntime->notify().connect(this, Notify::RO_BEFOREDELETE);
}

RDOPMDWatchPar::~RDOPMDWatchPar()
{}

void RDOPMDWatchPar::notify(std::size_t /*message*/, void* pParam)
{
	if ((ruint_type)pParam == m_resourceID)
	{
		m_resourceID = std::size_t(~0);
		m_timeErase = m_pRuntime->getCurrentTime();
	}
};

std::string RDOPMDWatchPar::traceValue() const
{
	return rdo::toString(m_currentValue.rdoValue);
}

void RDOPMDWatchPar::resetResult(const LPRDORuntime& pRuntime)
{
	ASSERT(pRuntime);

	m_pResource = pRuntime->getResourceByID(m_resourceID);
	ASSERT(m_pResource);

	m_currentValue = m_pResource->getParam(m_paramID);
	m_timePrev     = m_timeBegin = m_timeErase = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

void RDOPMDWatchPar::checkResult(const LPRDORuntime& pRuntime)
{
	if (!m_pResource)
		return;

	if (m_resourceID == std::size_t(~0))
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

void RDOPMDWatchPar::calcStat(const LPRDORuntime& pRuntime, std::ostream& stream)
{
	if (!m_wasFinalCalc)
	{
		checkResult(pRuntime);

		m_acc(m_currentValue.doubleValue, boost::accumulators::weight = m_currentValue.weight);

		m_wasFinalCalc = true;
	}

	double average = boost::accumulators::weighted_mean(m_acc);
	std::size_t count = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::weighted_variance(m_acc);

	bool averageEnable = count > 0 && fabs(variance) > DBL_EPSILON;
	bool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

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

	printLeft(stream, name());
	stream
		<< "\t" << "Тип:"        << "\t" << "par"
		<< "\t" << "Посл.знач.:" << "\t" << ResultStreamItem<std::string>(count > 0, traceValue())
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
RDOPMDWatchState::RDOPMDWatchState(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const LPRDOCalc& pLogic)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (pLogic               )
	, m_wasFinalCalc(false                )
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

std::string RDOPMDWatchState::traceValue() const
{
	return m_currentValue.state ? "TRUE" : "FALSE";
}

void RDOPMDWatchState::resetResult(const LPRDORuntime& pRuntime)
{
	try
	{
		m_currentValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	}
	catch (const RDOUndefinedException&)
	{
		m_currentValue = false;
	}
	m_timePrev     = m_timeBegin = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

void RDOPMDWatchState::checkResult(const LPRDORuntime& pRuntime)
{
	bool newValue;
	try
	{
		newValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	}
	catch(const RDOUndefinedException&)
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

void RDOPMDWatchState::calcStat(const LPRDORuntime& pRuntime, std::ostream& stream)
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
	double average = boost::accumulators::sum(m_acc) / (currTime - m_timeBegin);
	std::size_t count = boost::accumulators::count(m_acc);

	printLeft(stream, name());
	stream
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
RDOPMDWatchQuant::RDOPMDWatchQuant(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& /*resTypeName*/, int rtpID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (NULL                 )
	, m_rtpID       (rtpID                )
	, m_wasFinalCalc(false                )
{}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

std::string RDOPMDWatchQuant::traceValue() const
{
	return rdo::toString(m_currentValue.quant);
}

void RDOPMDWatchQuant::resetResult(const LPRDORuntime& pRuntime)
{
	m_currentValue = 0;
	m_timePrev     = m_timeBegin = pRuntime->getCurrentTime();
	m_wasFinalCalc = false;
}

std::size_t RDOPMDWatchQuant::calcCurrentQuant(const LPRDORuntime& pRuntime) const
{
	std::size_t newQuant = 0;
	RDORuntime::ResCIterator end = pRuntime->getResType(m_rtpID)->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->getResType(m_rtpID)->res_begin(); it != end; ++it)
	{
		if (*it == 0)
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

void RDOPMDWatchQuant::checkResult(const LPRDORuntime& pRuntime)
{
	std::size_t newQuant = calcCurrentQuant(pRuntime);

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

void RDOPMDWatchQuant::calcStat(const LPRDORuntime& pRuntime, std::ostream& stream)
{
	if (!m_wasFinalCalc)
	{
		std::size_t newQuant = calcCurrentQuant(pRuntime);
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

	double average = boost::accumulators::weighted_mean(m_acc);
	std::size_t count = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::weighted_variance(m_acc);

	bool averageEnable = count > 0 && fabs(variance) > DBL_EPSILON;
	bool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

	double stdDeviation = varianceEnable
		? sqrt(variance)
		: 0.0;

	double cv = averageEnable
		? (variance / average) * 100.0
		: 0.0;

	double median = count > 0
		? boost::accumulators::weighted_median(m_acc)
		: 0.0;

	printLeft(stream, name());
	stream
		<< "\t" << "Тип:"        << "\t" << "quant"
		<< "\t" << "Посл.знач.:" << "\t" << ResultStreamItem<std::string>(true, traceValue())
		<< "\t" << "Ср.знач.:"   << "\t" << ResultStreamItem<double> (true, average     )
		<< "\t" << "Мин.знач.:"  << "\t" << ResultStreamItem<int>    (true, (int)(boost::accumulators::min)(m_acc))
		<< "\t" << "Макс.знач.:" << "\t" << ResultStreamItem<int>    (true, (int)(boost::accumulators::max)(m_acc))
		<< "\t" << "Числ.наб.:"  << "\t" << count
		<< "\t" << "Стд.откл.:"  << "\t" << ResultStreamItem<double> (varianceEnable, stdDeviation)
		<< "\t" << "К.вар.%:"    << "\t" << ResultStreamItem<double> (averageEnable,  cv          )
		<< "\t" << "Медиана:"    << "\t" << ResultStreamItem<double> (count > 0,      median      )
		<< '\n';
}

void RDOPMDWatchQuant::setLogicCalc(const LPRDOCalc& pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(const LPRDORuntime& pRuntime, const std::string& name, bool trace, const std::string& /*resTypeName*/, int rtpID)
	: RDOPMDResult  (pRuntime, name, trace)
	, m_pLogicCalc  (NULL                 )
	, m_pArithmCalc (NULL                 )
	, m_rtpID       (rtpID                )
{
	m_wasChanged = false;
}

RDOPMDWatchValue::~RDOPMDWatchValue()
{}

std::string RDOPMDWatchValue::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchValue::resetResult(const LPRDORuntime& /*pRuntime*/)
{
	m_currValue = 0;
}

void RDOPMDWatchValue::checkResult(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPMDWatchValue::calcStat(const LPRDORuntime& /*pRuntime*/, std::ostream& stream)
{
	double average = boost::accumulators::mean(m_acc);
	std::size_t count = boost::accumulators::count(m_acc);
	double variance = boost::accumulators::variance(m_acc);

	bool averageEnable = count > 0 && fabs(variance) > DBL_EPSILON;
	bool varianceEnable = count > 0 && fabs(variance) > DBL_EPSILON;

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

	printLeft(stream, name());
	stream
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

void RDOPMDWatchValue::checkResourceErased(const LPRDOResource& pResource)
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

void RDOPMDWatchValue::setLogicCalc(const LPRDOCalc& pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

void RDOPMDWatchValue::setArithmCalc(const LPRDOCalc& pArithmCalc)
{
	m_pArithmCalc = pArithmCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(const LPRDORuntime& pRuntime, const std::string& name, const LPRDOCalc& pArithm)
	: RDOPMDResult  (pRuntime, name, false)
	, m_pArithmCalc (pArithm              )
	, m_wasFinalCalc(false                )
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

std::string RDOPMDGetValue::traceValue() const
{
	return "ERROR";
}

void RDOPMDGetValue::resetResult(const LPRDORuntime& /*pRuntime*/)
{
	m_wasFinalCalc = false;
}

void RDOPMDGetValue::checkResult(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPMDGetValue::calcStat(const LPRDORuntime& pRuntime, std::ostream& stream)
{
	if (!m_wasFinalCalc)
	{
		m_value        = m_pArithmCalc->calcValue(pRuntime);
		m_wasFinalCalc = true;
	}

	printLeft(stream, name());
	stream
		<< "\t" << "Тип:"      << "\t" << "get_value"
		<< "\t" << "Значение:" << "\t" << m_value.getAsString()
		<< '\n';
}

const RDOValue& RDOPMDGetValue::getValue() const
{
	return m_value;
}

void RDOPMDWatchPar::writeModelStructure(std::ostream& stream) const
{
	stream << traceId() << " watch_par" << std::endl;
}

void RDOPMDWatchState::writeModelStructure(std::ostream& stream) const
{
	stream << traceId() << " watch_state" << std::endl;
}

void RDOPMDWatchQuant::writeModelStructure(std::ostream& stream) const
{
	stream << traceId() << " watch_quant" << std::endl;
}

void RDOPMDWatchValue::writeModelStructure(std::ostream& stream) const
{
	stream << traceId() << " watch_value" << std::endl;
}

void RDOPMDGetValue::writeModelStructure(std::ostream& stream) const
{
	stream << traceId() << " get_value" << std::endl;
}

CLOSE_RDO_RUNTIME_NAMESPACE
