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

#if defined( ARCHITECTURES_AMD64 )
typedef ruint64 ruint_type;
#elif defined( ARCHITECTURES_X86 )
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
		stream << _T("нет данных");
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
	: RDOPMDResult(pRuntime, name, trace)
	, m_resourceID(resourceID           )
	, m_paramID   (paramID              )
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
	return rdo::toString(m_currValue);
}

void RDOPMDWatchPar::resetResult(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(pRuntime);

	m_pResource = pRuntime->getResourceByID(m_resourceID);
	ASSERT(m_pResource);

	m_currValue   = m_pResource->getParam(m_paramID);
	m_watchNumber = 0;
	m_sum         = 0;
	m_minValue    = m_currValue;
	m_maxValue    = m_currValue;
	m_timePrev    = m_timeBegin = m_timeErase = pRuntime->getCurrentTime();
}

void RDOPMDWatchPar::checkResult(CREF(LPRDORuntime) pRuntime)
{
	if (m_resourceID == ~0)
	{
		return;
	}

	ASSERT(m_pResource);
	RDOValue newValue = m_pResource->getParam(m_paramID);
	if (newValue != m_currValue)
	{
		double currTime = pRuntime->getCurrentTime();
		double val      = m_currValue.getDouble() * (currTime - m_timePrev);
		m_sum          += val;
		m_timePrev      = currTime;
		m_currValue     = newValue;
		m_wasChanged    = true;
		m_watchNumber++;

		if (m_minValue > m_currValue)
		{
			m_minValue = m_currValue;
		}

		if (m_maxValue < m_currValue)
		{
			m_maxValue = m_currValue;
		}
	}
}

void RDOPMDWatchPar::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	double currTime = m_resourceID == ~0 ? m_timeErase : pRuntime->getCurrentTime();
	double val      = m_currValue.getDouble() * (currTime - m_timePrev);
	m_sum          += val;
	double average  = m_sum / (currTime - m_timeBegin);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << _T("Тип:")        << _T("\t") << _T("par")
		<< _T("\t") << _T("Посл.знач.:") << _T("\t") << ResultStreamItem<tstring> (m_watchNumber > 0, traceValue())
		<< _T("\t") << _T("Ср.знач.:")   << _T("\t") << ResultStreamItem<double>  (m_watchNumber > 0, average     )
		<< _T("\t") << _T("Мин.знач.:")  << _T("\t") << ResultStreamItem<RDOValue>(m_watchNumber > 0, m_minValue  )
		<< _T("\t") << _T("Макс.знач.:") << _T("\t") << ResultStreamItem<RDOValue>(m_watchNumber > 0, m_maxValue  )
		<< _T("\t") << _T("Числ.наб.:")  << _T("\t") << m_watchNumber
		<< _T('\n');
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(LPRDOCalc) pLogic)
	: RDOPMDResult(pRuntime, name, trace)
	, m_pLogicCalc(pLogic               )
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

tstring RDOPMDWatchState::traceValue() const
{
	return m_currValue ? _T("TRUE") : _T("FALSE");
}

void RDOPMDWatchState::resetResult(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	try
	{
		m_currValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	}
	catch (CREF(RDOUndefinedException))
	{
		m_currValue = false;
	}
	m_sum      = 0;
	m_minValue = DBL_MAX;
	m_maxValue = DBL_MIN;
	m_timePrev = m_timeBegin = pRuntime->getCurrentTime();
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
	if (newValue && !m_currValue) //! from FALSE to TRUE
	{
		m_timePrev   = pRuntime->getCurrentTime();
		m_wasChanged = true;
	}
	else if (!newValue && m_currValue) //! from TRUE to FALSE
	{
		double currTime = pRuntime->getCurrentTime();
		double val      = currTime - m_timePrev;
		m_sum          += val;
		m_wasChanged    = true;
		m_watchNumber++;

		if (m_minValue > val)
		{
			m_minValue = val;
		}
		if (m_maxValue < val)
		{
			m_maxValue = val;
		}
	}
	m_currValue = newValue;
}

void RDOPMDWatchState::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	double currTime = pRuntime->getCurrentTime();
	double val      = m_currValue * (currTime - m_timePrev);
	m_sum          += val;
	double average  = m_sum / (currTime - m_timeBegin);

	if (m_watchNumber == 0)
	{
		m_minValue = 0.0;
		m_maxValue = 0.0;
	}

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << _T("Тип:")        << _T("\t") << _T("state")
		<< _T("\t") << _T("Посл.знач.:") << _T("\t") << traceValue()
		<< _T("\t") << _T("% соотв.:")   << _T("\t") << boost::format(_T("%1.6f")) % average
		<< _T("\t") << _T("Мин.длит.:")  << _T("\t") << boost::format(_T("%1.6f")) % m_minValue
		<< _T("\t") << _T("Макс.длит.:") << _T("\t") << boost::format(_T("%1.6f")) % m_maxValue
		<< _T("\t") << _T("Числ.наб.:")  << _T("\t") << m_watchNumber
		<< _T('\n');
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDResult(pRuntime, name, trace)
	, m_pLogicCalc(NULL                 )
	, m_rtpID     (rtpID                )
{
	UNUSED(resTypeName);
}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

tstring RDOPMDWatchQuant::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchQuant::resetResult(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	m_currValue   = -1;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;
	m_timePrev    = m_timeBegin = pRuntime->getCurrentTime();
}

void RDOPMDWatchQuant::checkResult(CREF(LPRDORuntime) pRuntime)
{
	int newValue = 0;
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != pRuntime->res_end(); it++)
	{
		if (*it == 0)
			continue;

		if (!(*it)->checkType(m_rtpID))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pLogicCalc->calcValue(pRuntime).getAsBool())
		{
			newValue++;
		}

		pRuntime->popGroupFunc();
	}

	if (newValue != m_currValue)
	{
		double currTime = pRuntime->getCurrentTime();
		double val      = m_currValue * (currTime - m_timePrev);
		m_sum          += val;
		m_sumSqr       += val * val;
		m_timePrev      = currTime;
		m_currValue     = newValue;
		m_wasChanged    = true;
		m_watchNumber++;

		if (m_minValue > m_currValue)
		{
			m_minValue = m_currValue;
		}

		if (m_maxValue < m_currValue)
		{
			m_maxValue = m_currValue;
		}
	}
}

void RDOPMDWatchQuant::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	double currTime = pRuntime->getCurrentTime();
	double val      = m_currValue * (currTime - m_timePrev);
	m_sum          += val;
	m_sumSqr       += val * val;
	double average  = m_sum / (currTime - m_timeBegin);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << ResultStreamItem<tstring>(m_watchNumber > 0, traceValue())
		<< _T("\t") << m_watchNumber
		<< _T("\t") << ResultStreamItem<double>(m_watchNumber > 0, average   )
		<< _T("\t") << ResultStreamItem<double>(m_watchNumber > 0, m_sumSqr  )
		<< _T("\t") << ResultStreamItem<double>(m_watchNumber > 0, m_minValue)
		<< _T("\t") << ResultStreamItem<double>(m_watchNumber > 0, m_maxValue)
		<< _T('\n');
}

void RDOPMDWatchQuant::setLogicCalc(CREF(LPRDOCalc) pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDResult (pRuntime, name, trace)
	, m_pLogicCalc (NULL                 )
	, m_pArithmCalc(NULL                 )
	, m_rtpID      (rtpID                )
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

	m_watchNumber = 0;
	m_currValue   = 0;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;
}

void RDOPMDWatchValue::checkResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPMDWatchValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	UNUSED(pRuntime);

	double average, averageSqr, deviation;
	if (m_watchNumber < 2)
	{
		average = averageSqr = deviation = 0;
	}
	else
	{
		average    = m_sum / m_watchNumber;
		averageSqr = m_sumSqr - 2 * average * m_sum + m_watchNumber * average * average;
		averageSqr = sqrt(averageSqr / (m_watchNumber - 1));
		/// @todo а почему корень берем от m_watchNumber?
		deviation  = averageSqr / sqrt((double)m_watchNumber);
	}

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << ResultStreamItem<double>  (m_watchNumber > 0, average   )
		<< _T("\t") << ResultStreamItem<double>  (m_watchNumber > 0, averageSqr)
		<< _T("\t") << ResultStreamItem<double>  (m_watchNumber > 0, deviation )
		<< _T("\t") << ResultStreamItem<RDOValue>(m_watchNumber > 0, m_minValue)
		<< _T("\t") << ResultStreamItem<RDOValue>(m_watchNumber > 0, m_maxValue)
		<< _T('\n');
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
		double curr = m_currValue.getDouble();
		m_sum    += curr;
		m_sumSqr += curr * curr;
		m_watchNumber++;
		if (m_minValue > m_currValue)
		{
			m_minValue = m_currValue;
		}
		if (m_maxValue < m_currValue)
		{
			m_maxValue = m_currValue;
		}
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
	: RDOPMDResult (pRuntime, name, false)
	, m_pArithmCalc(pArithm              )
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

tstring RDOPMDGetValue::traceValue() const
{
	return _T("ERROR");
}

void RDOPMDGetValue::resetResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPMDGetValue::checkResult(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPMDGetValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(rdo::ostream) stream)
{
	m_value = m_pArithmCalc->calcValue(pRuntime);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << m_value.getAsString()
		<< _T('\n');
}

CREF(RDOValue) RDOPMDGetValue::getValue() const
{
	return m_value;
}

void RDOPMDWatchPar::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << _T(" watch_par") << std::endl;
}

void RDOPMDWatchState::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << _T(" watch_state") << std::endl;
}

void RDOPMDWatchQuant::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << _T(" watch_quant") << std::endl;
}

void RDOPMDWatchValue::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << _T(" watch_value") << std::endl;
}

void RDOPMDGetValue::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << traceId() << _T(" get_value") << std::endl;
}

CLOSE_RDO_RUNTIME_NAMESPACE
