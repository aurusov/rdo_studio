#ifndef RDOPOKAZ_H
#define RDOPOKAZ_H

#include "rdotrace.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOPokazTrace
{
public:
	RDOPMDPokaz( RDORuntime* sim, const std::string& name, bool trace );
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(RDOResource *res) { return false; }
	virtual void writePokazStructure( std::ostream& stream ) const = 0;
	const std::string& name() const { return m_name; }

protected:
	std::string m_name;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
public:
	RDOPMDWatchPar( RDORuntime* sim, const std::string& name, bool trace, const std::string& resName, const std::string& parName, int resNumber, int parNumber );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;

private:
	int       m_resNumber;
	int       m_parNumber;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	double    m_timeErase;

	virtual void notify( RDORuntimeObject* from, unsigned int message, void* param = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
public:
	RDOPMDWatchState( RDORuntime* sim, const std::string& name, bool trace, RDOCalc* logic );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;

private:
	RDOCalc* m_logicCalc;

	int      m_watchNumber;
	bool     m_currValue;
	double   m_sum;
	double   m_sumSqr;
	double   m_minValue;
	double   m_maxValue;

	double   m_timeBegin;
	double   m_timePrev;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz
{
public:
	RDOPMDWatchQuant( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;
	void setLogicCalc( RDOCalc* logicCalc ) { m_logicCalc = logicCalc; }

private:
	RDOCalc* m_logicCalc;
	int      m_rtp_id;

	int      m_watchNumber;
	int      m_currValue;
	double   m_sum;
	double   m_sumSqr;
	double   m_minValue;
	double   m_maxValue;

	double   m_timeBegin;
	double   m_timePrev;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz
{
public:
	RDOPMDWatchValue( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(RDOResource *res);
	void writePokazStructure( std::ostream& stream ) const;
	void setLogicCalc ( RDOCalc* logicCalc )   { m_logicCalc  = logicCalc;  }
	void setArithmCalc( RDOCalc* arithmCalc )  { m_arithmCalc = arithmCalc; }

private:
	RDOCalc*  m_logicCalc;
	RDOCalc*  m_arithmCalc;
	int       m_rtp_id;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
public:
	RDOPMDGetValue( RDORuntime* sim, const std::string& name, RDOCalc* arithm );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;

private:
	RDOCalc*  m_arithmCalc;
};

} // rdoRuntime

#endif // RDOPOKAZ_H
