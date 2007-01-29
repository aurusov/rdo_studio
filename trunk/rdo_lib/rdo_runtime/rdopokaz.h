#ifndef RDORUNTIMEPOKAZ_H
#define RDORUNTIMEPOKAZ_H

#include "rdotrace.h"
#include "rdoruntime.h"

namespace rdoParse 
{
class RDOFUNLogic;
class RDOFUNArithm;
class RDOPMDPokaz;
class RDOPMDWatchPar;
class RDOPMDWatchQuant;
class RDOPMDWatchValue;
}

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOPokazTrace
{
friend class rdoParse::RDOPMDPokaz;

protected:
	const std::string const name;

public:
	RDOPMDPokaz( const std::string* const _name, bool _trace );
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(rdoRuntime::RDOResource *res) { return false; }
	virtual void writePokazStructure(std::ostream &stream) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
friend class rdoParse::RDOPMDWatchPar;

private:
	int resNumber;
	int parNumber;

	int watchNumber;
	double currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchPar( std::string* _name, bool _trace, std::string* _resName, std::string* _parName );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
private:
	rdoRuntime::RDOCalc *logicCalc;

	int watchNumber;
	bool currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchState( std::string* _name, bool _trace, rdoParse::RDOFUNLogic* _logic );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz
{
friend class rdoParse::RDOPMDWatchQuant;

private:
	rdoParse::RDOFUNGroup *funGroup;
	rdoRuntime::RDOCalc *logicCalc;

	int watchNumber;
	int currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchQuant( std::string* _name, bool _trace, std::string* _resTypeName );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz
{
friend class rdoParse::RDOPMDWatchValue;

private:
	rdoParse::RDOFUNGroup *funGroup;
	rdoRuntime::RDOCalc *logicCalc;
	rdoRuntime::RDOCalc *arithmCalc;

	int watchNumber;
	double currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

public:
	RDOPMDWatchValue( std::string* _name, bool _trace, std::string* _resTypeName );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(rdoRuntime::RDOResource *res);
	void writePokazStructure(std::ostream &stream) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
private:
	rdoRuntime::RDOCalc *arithmCalc;

public:
	RDOPMDGetValue( std::string* _name, rdoParse::RDOFUNArithm* _arithm );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

} // rdoRuntime

#endif // RDORUNTIMEPOKAZ_H
