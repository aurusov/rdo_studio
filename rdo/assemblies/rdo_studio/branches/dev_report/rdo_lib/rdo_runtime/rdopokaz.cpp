#include "pch.h"
#include "rdopokaz.h"
#include "rdocalc.h"
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz( RDORuntime* sim, const std::string& name, bool trace ):
	RDOPokazTrace( sim, trace ),
	m_name( name )
{
	setTrace( trace );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar( RDORuntime* sim, const std::string& name, bool trace, const std::string& resName, const std::string& parName, int resNumber, int parNumber ):
	RDOPMDPokaz( sim, name, trace ),
	m_resNumber( resNumber ),
	m_parNumber( parNumber )
{
	static_cast<RDORuntime*>(sim)->connect( this, RDORuntime::RO_BEFOREDELETE );
}

void RDOPMDWatchPar::notify( RDORuntimeObject* from, unsigned int message, void* param )
{
	if ( (int)param == m_resNumber )
	{
		m_resNumber = -1;
		m_timeErase = getRuntime()->getCurrentTime();
	}
};

std::string RDOPMDWatchPar::traceValue()
{
	return rdo::toString( m_currValue );
}

bool RDOPMDWatchPar::resetPokaz(RDOSimulator *sim)
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

bool RDOPMDWatchPar::checkPokaz(RDOSimulator *sim)
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

bool RDOPMDWatchPar::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = m_resNumber == -1 ? m_timeErase : runtime->getCurrentTime();
	double val = m_currValue.getDouble() * (currTime - m_timePrev);
	m_sum	 += val;
	m_sumSqr += val * val;

	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << m_name 
		<< "\t" << traceValue()
		<< "\t" << m_watchNumber
		<< "\t" << average 
		<< "\t" << m_sumSqr 
		<< "\t" << m_minValue 
		<< "\t" << m_maxValue << '\n'; 

	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState( RDORuntime* sim, const std::string& name, bool trace, RDOCalc* logic ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( logic )
{
}

std::string RDOPMDWatchState::traceValue()
{
	return m_currValue ? "TRUE" : "FALSE";
}

bool RDOPMDWatchState::resetPokaz(RDOSimulator *sim)
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

bool RDOPMDWatchState::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);
	bool newValue = fabs(m_logicCalc->calcValue( runtime ).getDouble()) > DBL_EPSILON;
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

bool RDOPMDWatchState::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = m_currValue * (currTime - m_timePrev);
	m_sum    += val;
	m_sumSqr += val * val;

	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left
		<< m_name 
		<< "\t" << traceValue() 
		<< "\t" << m_watchNumber
		<< "\t" << average 
		<< "\t" << m_sumSqr 
		<< "\t" << m_minValue 
		<< "\t" << m_maxValue << '\n'; 

	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( NULL ),
	m_rtp_id( rtp_id )
{
}

std::string RDOPMDWatchQuant::traceValue()
{
	return rdo::toString( m_currValue );
}

bool RDOPMDWatchQuant::resetPokaz(RDOSimulator *sim)
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

bool RDOPMDWatchQuant::checkPokaz(RDOSimulator *sim)
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

bool RDOPMDWatchQuant::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = m_currValue * (currTime - m_timePrev);
	m_sum     += val;
	m_sumSqr  += val * val;
	double average = m_sum / (currTime - m_timeBegin);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << m_name 
		<< "\t" << traceValue()
		<< "\t" << m_watchNumber
		<< "\t" << average 
		<< "\t" << m_sumSqr 
		<< "\t" << m_minValue 
		<< "\t" << m_maxValue << '\n'; 

	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id ):
	RDOPMDPokaz( sim, name, trace ),
	m_logicCalc( NULL ),
	m_arithmCalc( NULL ),
	m_rtp_id( rtp_id )
{
	m_wasChanged = false;
}

std::string RDOPMDWatchValue::traceValue()
{
	return rdo::toString( m_currValue );
}

bool RDOPMDWatchValue::resetPokaz(RDOSimulator *sim)
{
	m_watchNumber = 0;
	m_currValue   = 0;
	m_sum         = 0;
	m_sumSqr      = 0;
	m_minValue    = DBL_MAX;
	m_maxValue    = DBL_MIN;

	return true;
}

bool RDOPMDWatchValue::checkPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDWatchValue::calcStat(RDOSimulator *sim)
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
	runtime->getResults() << std::left << m_name 
		<< "\t" << m_watchNumber
		<< "\t" << average 
		<< "\t" << averageSqr
		<< "\t" << deviation
		<< "\t" << m_minValue 
		<< "\t" << m_maxValue << '\n'; 

	return true;
}

bool RDOPMDWatchValue::checkResourceErased( rdoRuntime::RDOResource* res )
{
	if ( !res->checkType(m_rtp_id) ) {
		return false;
	}
	getRuntime()->pushGroupFunc(res);
	if ( m_logicCalc->calcValue( getRuntime() ).getAsBool() ) {
		m_currValue = m_arithmCalc->calcValue( getRuntime() );
		tracePokaz();
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
		return true;
	}
	getRuntime()->popGroupFunc();
	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue( RDORuntime* sim, const std::string& name, RDOCalc* arithm ):
	RDOPMDPokaz( sim, name, false ),
	m_arithmCalc( arithm )
{
}

std::string RDOPMDGetValue::traceValue()
{
	return "ERROR";
}

bool RDOPMDGetValue::resetPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDGetValue::checkPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDGetValue::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	runtime->getResults().width(30);
	runtime->getResults() << std::left << m_name 
		<< "\t" << m_arithmCalc->calcValue( runtime ).getAsString() << '\n';

	return true;
}

void RDOPMDWatchPar::writePokazStructure( std::ostream& stream ) const
{
	stream << traceId() << " watch_par" << std::endl;
}

void RDOPMDWatchState::writePokazStructure( std::ostream& stream ) const
{
	stream << traceId() << " watch_state" << std::endl;
}

void RDOPMDWatchQuant::writePokazStructure( std::ostream& stream ) const
{
	stream << traceId() << " watch_quant" << std::endl;
}

void RDOPMDWatchValue::writePokazStructure( std::ostream& stream ) const
{
	stream << traceId() << " watch_value" << std::endl;
}

void RDOPMDGetValue::writePokazStructure( std::ostream& stream ) const
{
	stream << traceId() << " get_value" << std::endl;
}

} // rdoRuntime
