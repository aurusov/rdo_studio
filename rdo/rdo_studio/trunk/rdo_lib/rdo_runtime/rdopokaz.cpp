/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "pch.h"
// ====================================================================== INCLUDES
#include <limits>
// ====================================================================== SYNOPSIS
#include "rdopokaz.h"
#include "rdocalc.h"
#include "rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz( RDORuntime* sim, const tstring& name, rbool trace ):
	RDOPokazTrace( sim, trace ),
	m_name( name )
{
	setTrace( trace );
}

CREF(tstring) RDOPMDPokaz::name() const
{
	return m_name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar( RDORuntime* sim, const tstring& name, rbool trace, const tstring& resName, const tstring& parName, int resNumber, int parNumber ):
	RDOPMDPokaz( sim, name, trace ),
	m_resNumber( resNumber ),
	m_parNumber( parNumber )
{
	static_cast<RDORuntime*>(sim)->connect( this, RDORuntime::RO_BEFOREDELETE );
}

void RDOPMDWatchPar::notify(ruint message, PTR(void) param)
{
	if ( (int)param == m_resNumber )
	{
		m_resNumber = -1;
		m_timeErase = getRuntime()->getCurrentTime();
	}
};

tstring RDOPMDWatchPar::traceValue() const
{
	return rdo::toString( m_currValue );
}

rbool RDOPMDWatchPar::resetPokaz(PTR(RDOSimulator) sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	m_watchNumber = 0;
	m_currValue   = runtime->getResParamVal( m_resNumber, m_parNumber );
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = m_currValue;
	m_maxValue    = m_currValue;

	m_timePrev    = m_timeBegin = m_timeErase = runtime->getCurrentTime();
	return true;
}

rbool RDOPMDWatchPar::checkPokaz(PTR(RDOSimulator) sim)
{
	if ( m_resNumber == -1 ) return false;
	rdoRuntime::RDORuntime* runtime = static_cast< rdoRuntime::RDORuntime* >(sim);
	RDOValue newValue = runtime->getResParamVal( m_resNumber, m_parNumber );
	if ( newValue != m_currValue ) {
		double currTime = runtime->getCurrentTime();
		double val = m_currValue.getDouble() * (currTime - m_timePrev);
		m_sum	    += val;
		m_sumSqr    += val * val;
		m_timePrev   = currTime;
		m_currValue  = newValue;
		m_wasChanged = true;
		m_watchNumber++;
		if ( m_minValue > m_currValue ) {
			m_minValue = m_currValue;
		}
		if ( m_maxValue < m_currValue ) {
			m_maxValue = m_currValue;
		}
		return true;
	}
	return false;
}

rbool RDOPMDWatchPar::calcStat(PTR(RDOSimulator) sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = m_resNumber == -1 ? m_timeErase : runtime->getCurrentTime();
	double val = m_currValue.getDouble() * (currTime - m_timePrev);
	m_sum	 += val;
	m_sumSqr += val * val;

	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << name() 
		<< _T("\t") << traceValue()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average 
		<< _T("\t") << m_sumSqr 
		<< _T("\t") << m_minValue 
		<< _T("\t") << m_maxValue << _T('\n');

	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState( RDORuntime* sim, const tstring& name, rbool trace, RDOCalc* logic ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( logic )
{
}

tstring RDOPMDWatchState::traceValue() const
{
	return m_currValue ? _T("TRUE") : _T("FALSE");
}

rbool RDOPMDWatchState::resetPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	m_watchNumber = 0;
	m_currValue   = fabs(m_logicCalc->calcValue( runtime ).getDouble()) > DBL_EPSILON;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;

	m_timePrev    = m_timeBegin = runtime->getCurrentTime();
	return true;
}

rbool RDOPMDWatchState::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);
	rbool newValue = fabs(m_logicCalc->calcValue( runtime ).getDouble()) > DBL_EPSILON;
	if ( newValue && !m_currValue ) { // from FALSE to TRUE
		m_timePrev   = runtime->getCurrentTime();
		m_wasChanged = true;
	} else if ( !newValue && m_currValue ) { // from TRUE to FALSE
		double currTime = runtime->getCurrentTime();
		double val = currTime - m_timePrev;
		m_sum	 += val;
		m_sumSqr += val * val;
		m_wasChanged = true;
		m_watchNumber++;
		if ( m_minValue > val ) {
			m_minValue = val;
		}
		if ( m_maxValue < val ) {
			m_maxValue = val;
		}
	}
	m_currValue = newValue;
	return true;
}

rbool RDOPMDWatchState::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = m_currValue * (currTime - m_timePrev);
	m_sum    += val;
	m_sumSqr += val * val;

	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left
		<< name() 
		<< _T("\t") << traceValue() 
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average 
		<< _T("\t") << m_sumSqr 
		<< _T("\t") << m_minValue 
		<< _T("\t") << m_maxValue << _T('\n');

	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant( RDORuntime* sim, const tstring& name, rbool trace, const tstring& resTypeName, int rtp_id ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( NULL ),
	m_rtp_id( rtp_id )
{
}

tstring RDOPMDWatchQuant::traceValue() const
{
	return rdo::toString( m_currValue );
}

rbool RDOPMDWatchQuant::resetPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	m_watchNumber = 0;
	m_currValue   = -1;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;

	m_timePrev    = m_timeBegin = runtime->getCurrentTime();
	return true;
}

rbool RDOPMDWatchQuant::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	int newValue = 0;
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != runtime->res_end(); it++ )
	{
		if(*it == NULL)
			continue;

		if( !(*it)->checkType(m_rtp_id) )
			continue;

		runtime->pushGroupFunc(*it);
		if(m_logicCalc->calcValue( runtime ).getAsBool())
			newValue++;

		runtime->popGroupFunc();
	}

	if(newValue != m_currValue)
	{
		double currTime = runtime->getCurrentTime();
		double val = m_currValue * (currTime - m_timePrev);
		m_sum      += val;
		m_sumSqr   += val * val;
		m_timePrev  = currTime;

		m_currValue = newValue;
		m_wasChanged = true;
		m_watchNumber ++;
		if (m_minValue > m_currValue)
			m_minValue = m_currValue;

		if (m_maxValue < m_currValue)
			m_maxValue = m_currValue;

		return true;
	}
	return false;
}

rbool RDOPMDWatchQuant::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = m_currValue * (currTime - m_timePrev);
	m_sum     += val;
	m_sumSqr  += val * val;
	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << name() 
		<< _T("\t") << traceValue()
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average 
		<< _T("\t") << m_sumSqr 
		<< _T("\t") << m_minValue 
		<< _T("\t") << m_maxValue << _T('\n');

	return true;
}

void RDOPMDWatchQuant::setLogicCalc(PTR(RDOCalc) logicCalc)
{
	m_logicCalc = logicCalc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue( RDORuntime* sim, const tstring& name, rbool trace, const tstring& resTypeName, int rtp_id ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( NULL ),
	m_arithmCalc( NULL ),
	m_rtp_id( rtp_id )
{
	m_wasChanged = false;
}

tstring RDOPMDWatchValue::traceValue() const
{
	return rdo::toString( m_currValue );
}

rbool RDOPMDWatchValue::resetPokaz(RDOSimulator *sim)
{
	m_watchNumber = 0;
	m_currValue   = 0;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;

	return true;
}

rbool RDOPMDWatchValue::checkPokaz(RDOSimulator *sim)
{
	return false;
}

rbool RDOPMDWatchValue::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double average, averageSqr, deviation;
	if (m_watchNumber < 2)
		average = averageSqr = deviation = 0;
	else
	{
		average    = m_sum / m_watchNumber;
		averageSqr = m_sumSqr - 2 * average * m_sum + m_watchNumber * average * average;
		averageSqr = sqrt(averageSqr / (m_watchNumber - 1));
		deviation  = averageSqr / sqrt((double)m_watchNumber); // qq а почему корень берем от m_watchNumber ?
	}

	runtime->getResults().width(30);
	runtime->getResults() << std::left << name() 
		<< _T("\t") << m_watchNumber
		<< _T("\t") << average 
		<< _T("\t") << averageSqr
		<< _T("\t") << deviation
		<< _T("\t") << m_minValue 
		<< _T("\t") << m_maxValue << _T('\n');

	return true;
}

void RDOPMDWatchValue::checkResourceErased( rdoRuntime::RDOResource* res )
{
	if (!res->checkType(m_rtp_id))
	{
		return;
	}
	getRuntime()->pushGroupFunc(res);
	if ( m_logicCalc->calcValue( getRuntime() ).getAsBool() ) {
		m_currValue = m_arithmCalc->calcValue( getRuntime() );
		tracePokaz(); // TODO: убрать отсюда ? (и перенести DECLARE_IPokazTrace в приват)
//		runtime->getTracer()->writePokaz(runtime, this);
		double curr = m_currValue.getDouble();
		m_sum    += curr;
		m_sumSqr += curr * curr;
		m_watchNumber++;
		if ( m_minValue > m_currValue ) {
			m_minValue = m_currValue;
		}
		if ( m_maxValue < m_currValue ) {
			m_maxValue = m_currValue;
		}
		return;
	}
	getRuntime()->popGroupFunc();
	return;
}

void RDOPMDWatchValue::setLogicCalc(PTR(rdoRuntime::RDOCalc) logicCalc)
{
	m_logicCalc = logicCalc;
}

void RDOPMDWatchValue::setArithmCalc(PTR(rdoRuntime::RDOCalc) arithmCalc)
{
	m_arithmCalc = arithmCalc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(PTR(RDORuntime) sim, const tstring& name, RDOCalc* arithm):
	RDOPMDPokaz( sim, name, false ),
	m_arithmCalc( arithm )
{
}

tstring RDOPMDGetValue::traceValue() const
{
	return _T("ERROR");
}

rbool RDOPMDGetValue::resetPokaz(PTR(RDOSimulator) sim)
{
	return false;
}

rbool RDOPMDGetValue::checkPokaz(PTR(RDOSimulator) sim)
{
	return false;
}

rbool RDOPMDGetValue::calcStat(PTR(RDOSimulator) sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << name() 
		<< _T("\t") << m_arithmCalc->calcValue( runtime ).getAsString() << _T('\n');

	return true;
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
