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
protected:
	std::string name;

public:
	RDOPMDPokaz( RDOSimulatorTrace* _sim, const std::string& _name, bool _trace );
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(RDOResource *res) { return false; }
	virtual void writePokazStructure( std::ostream& stream ) const = 0;
	void setID( int _id ) { id = _id; }
	const std::string& getName() const { return name; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
private:
	int resNumber;
	int parNumber;

	int watchNumber;
	RDOValue currValue;
	double sum;
	double sumSqr;
	RDOValue minValue;
	RDOValue maxValue;

	double timeBegin;
	double timePrev;

	double timeErase;

	virtual void notify( RDORuntimeObject* from, UINT message, void* param = NULL );

public:
	RDOPMDWatchPar( RDOSimulatorTrace* _sim, const std::string& _name, bool _trace, const std::string& _resName, const std::string& _parName, int _resNumber, int _parNumber );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
private:
	RDOCalc* logicCalc;

	int watchNumber;
	bool currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchState( RDOSimulatorTrace* _sim, const std::string& _name, bool _trace, RDOCalc* _logic );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz
{
private:
	RDOCalc* logicCalc;
	int rtp_id;

	int watchNumber;
	int currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchQuant( RDOSimulatorTrace* _sim, const std::string& _name, bool _trace, const std::string& _resTypeName, int _rtp_id );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;
	void setLogicCalc( RDOCalc* _logicCalc ) { logicCalc = _logicCalc; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz
{
private:
	RDOCalc *logicCalc;
	RDOCalc *arithmCalc;
	int rtp_id;

	int watchNumber;
	RDOValue currValue;
	double sum;
	double sumSqr;
	RDOValue minValue;
	RDOValue maxValue;

public:
	RDOPMDWatchValue( RDOSimulatorTrace* _sim, const std::string& _name, bool _trace, const std::string& _resTypeName, int _rtp_id );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(RDOResource *res);
	void writePokazStructure( std::ostream& stream ) const;
	void setLogicCalc( RDOCalc* _logicCalc )   { logicCalc = _logicCalc;   }
	void setArithmCalc( RDOCalc* _arithmCalc ) { arithmCalc = _arithmCalc; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
private:
	RDOCalc* arithmCalc;

public:
	RDOPMDGetValue( RDOSimulatorTrace* _sim, const std::string& _name, RDOCalc* _arithm );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure( std::ostream& stream ) const;
};

} // rdoRuntime

#endif // RDOPOKAZ_H
