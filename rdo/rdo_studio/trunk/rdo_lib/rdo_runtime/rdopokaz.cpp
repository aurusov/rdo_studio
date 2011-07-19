/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
#include <limits>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdopokaz.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace)
	: RDOPokazTrace(pRuntime, trace)
	, m_name       (name           )
{
	setTrace(trace);
}

RDOPMDPokaz::~RDOPMDPokaz()
{}

CREF(tstring) RDOPMDPokaz::name() const
{
	return m_name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resName, CREF(tstring) parName, int resNumber, int parNumber)
	: RDOPMDPokaz(pRuntime, name, trace)
	, m_resNumber(resNumber            )
	, m_parNumber(parNumber            )
{
	pRuntime->connect(this, RDORuntime::RO_BEFOREDELETE);
}

RDOPMDWatchPar::~RDOPMDWatchPar()
{}

void RDOPMDWatchPar::notify(ruint message, PTR(void) param)
{
	if ((int)param == m_resNumber)
	{
		m_resNumber = -1;
		m_timeErase = m_pRuntime->getCurrentTime();
	}
};

tstring RDOPMDWatchPar::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchPar::resetPokaz(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	m_currValue   = pRuntime->getResParamVal(m_resNumber, m_parNumber);
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = m_currValue;
	m_maxValue    = m_currValue;
	m_timePrev    = m_timeBegin = m_timeErase = pRuntime->getCurrentTime();
}

void RDOPMDWatchPar::checkPokaz(CREF(LPRDORuntime) pRuntime)
{
	if (m_resNumber == -1)
	{
		return;
	}
	RDOValue newValue = pRuntime->getResParamVal(m_resNumber, m_parNumber);
	if (newValue != m_currValue)
	{
		double currTime = pRuntime->getCurrentTime();
		double val      = m_currValue.getDouble() * (currTime - m_timePrev);
		m_sum	       += val;
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

void RDOPMDWatchPar::calcStat(CREF(LPRDORuntime) pRuntime, REF(std::ostream) stream)
{
	double currTime = m_resNumber == -1 ? m_timeErase : pRuntime->getCurrentTime();
	double val      = m_currValue.getDouble() * (currTime - m_timePrev);
	m_sum	       += val;
	m_sumSqr       += val * val;
	double average  = m_sum / (currTime - m_timeBegin);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << traceValue()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average
		<< _T("\t") << m_sumSqr
		<< _T("\t") << m_minValue
		<< _T("\t") << m_maxValue << _T('\n');
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(LPRDOCalc) pLogic)
	: RDOPMDPokaz (pRuntime, name, trace)
	, m_pLogicCalc(pLogic               )
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

tstring RDOPMDWatchState::traceValue() const
{
	return m_currValue ? _T("TRUE") : _T("FALSE");
}

void RDOPMDWatchState::resetPokaz(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	m_currValue   = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;
	m_timePrev    = m_timeBegin = pRuntime->getCurrentTime();
}

void RDOPMDWatchState::checkPokaz(CREF(LPRDORuntime) pRuntime)
{
	rbool newValue = fabs(m_pLogicCalc->calcValue(pRuntime).getDouble()) > DBL_EPSILON;
	if (newValue && !m_currValue) //! from FALSE to TRUE
	{
		m_timePrev   = pRuntime->getCurrentTime();
		m_wasChanged = true;
	}
	else if (!newValue && m_currValue) //! from TRUE to FALSE
	{
		double currTime = pRuntime->getCurrentTime();
		double val      = currTime - m_timePrev;
		m_sum	       += val;
		m_sumSqr       += val * val;
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

void RDOPMDWatchState::calcStat(CREF(LPRDORuntime) pRuntime, REF(std::ostream) stream)
{
	double currTime = pRuntime->getCurrentTime();
	double val      = m_currValue * (currTime - m_timePrev);
	m_sum          += val;
	m_sumSqr       += val * val;
	double average  = m_sum / (currTime - m_timeBegin);

	stream.width(30);
	stream << std::left
		<< name()
		<< _T("\t") << traceValue()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average
		<< _T("\t") << m_sumSqr
		<< _T("\t") << m_minValue
		<< _T("\t") << m_maxValue << _T('\n');
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDPokaz (pRuntime, name, trace)
	, m_pLogicCalc(NULL                 )
	, m_rtpID     (rtpID                )
{}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

tstring RDOPMDWatchQuant::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchQuant::resetPokaz(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	m_currValue   = -1;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;
	m_timePrev    = m_timeBegin = pRuntime->getCurrentTime();
}

void RDOPMDWatchQuant::checkPokaz(CREF(LPRDORuntime) pRuntime)
{
	int newValue = 0;
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != pRuntime->res_end(); it++)
	{
		if (*it == NULL)
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

void RDOPMDWatchQuant::calcStat(CREF(LPRDORuntime) pRuntime, REF(std::ostream) stream)
{
	double currTime = pRuntime->getCurrentTime();
	double val      = m_currValue * (currTime - m_timePrev);
	m_sum          += val;
	m_sumSqr       += val * val;
	double average  = m_sum / (currTime - m_timeBegin);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << traceValue()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average
		<< _T("\t") << m_sumSqr
		<< _T("\t") << m_minValue
		<< _T("\t") << m_maxValue << _T('\n');
}

void RDOPMDWatchQuant::setLogicCalc(CREF(LPRDOCalc) pLogicCalc)
{
	m_pLogicCalc = pLogicCalc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID)
	: RDOPMDPokaz  (pRuntime, name, trace)
	, m_pLogicCalc (NULL                 )
	, m_pArithmCalc(NULL                 )
	, m_rtpID      (rtpID                )
{
	m_wasChanged = false;
}

RDOPMDWatchValue::~RDOPMDWatchValue()
{}

tstring RDOPMDWatchValue::traceValue() const
{
	return rdo::toString(m_currValue);
}

void RDOPMDWatchValue::resetPokaz(CREF(LPRDORuntime) pRuntime)
{
	m_watchNumber = 0;
	m_currValue   = 0;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;
}

void RDOPMDWatchValue::checkPokaz(CREF(LPRDORuntime) pRuntime)
{}

void RDOPMDWatchValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(std::ostream) stream)
{
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
		deviation  = averageSqr / sqrt((double)m_watchNumber); // qq а почему корень берем от m_watchNumber ?
	}

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average
		<< _T("\t") << averageSqr
		<< _T("\t") << deviation
		<< _T("\t") << m_minValue
		<< _T("\t") << m_maxValue << _T('\n');
}

void RDOPMDWatchValue::checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource)
{
	if (!pResource->checkType(m_rtpID))
	{
		return;
	}
	m_pRuntime->pushGroupFunc(pResource);
	if (m_pLogicCalc->calcValue(m_pRuntime).getAsBool())
	{
		m_currValue = m_pArithmCalc->calcValue(m_pRuntime);
		tracePokaz(); /// @todo убрать отсюда ? (и перенести DECLARE_IPokazTrace в приват)
//		pRuntime->getTracer()->writePokaz(pRuntime, this);
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, CREF(LPRDOCalc) pArithm)
	: RDOPMDPokaz  (pRuntime, name, false)
	, m_pArithmCalc(pArithm              )
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

tstring RDOPMDGetValue::traceValue() const
{
	return _T("ERROR");
}

void RDOPMDGetValue::resetPokaz(CREF(LPRDORuntime) pRuntime)
{}

void RDOPMDGetValue::checkPokaz(CREF(LPRDORuntime) pRuntime)
{}

void RDOPMDGetValue::calcStat(CREF(LPRDORuntime) pRuntime, REF(std::ostream) stream)
{
	m_value = m_pArithmCalc->calcValue(pRuntime);

	stream.width(30);
	stream << std::left << name()
		<< _T("\t") << m_value.getAsString() << _T('\n');
}

CREF(RDOValue) RDOPMDGetValue::getValue() const
{
	return m_value;
}

void RDOPMDWatchPar::writeModelStructure(REF(std::ostream) stream) const
{
	stream << traceId() << _T(" watch_par") << std::endl;
}

void RDOPMDWatchState::writeModelStructure(REF(std::ostream) stream) const
{
	stream << traceId() << _T(" watch_state") << std::endl;
}

void RDOPMDWatchQuant::writeModelStructure(REF(std::ostream) stream) const
{
	stream << traceId() << _T(" watch_quant") << std::endl;
}

void RDOPMDWatchValue::writeModelStructure(REF(std::ostream) stream) const
{
	stream << traceId() << _T(" watch_value") << std::endl;
}

void RDOPMDGetValue::writeModelStructure(REF(std::ostream) stream) const
{
	stream << traceId() << _T(" get_value") << std::endl;
}

CLOSE_RDO_RUNTIME_NAMESPACE
