#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdoStdFuncs.h"
#include "rdotrace.h"

namespace rdoRuntime
{
class RDOCalc;
class RDOResource;
}

using namespace rdoRuntime;

namespace rdoParse 
{
struct RDOFUNLogic;
struct RDOFUNGroup;
struct RDOFUNArithm;

//////////////////////////// class RDOPMDPokaz /////////////////////////////////
class RDOPMDPokaz: public RDOPokazTrace
{
protected:
	const string const name;
public:
	RDOPMDPokaz(const string *const _name, bool _trace);
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(RDOResource *res) { return false; }
};

//////////////////////////// class RDOPMDWatchPar /////////////////////////////////
class RDOPMDWatchPar: public RDOPMDPokaz
{
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
	RDOPMDWatchPar(string *_name, bool _trace, string *_resName, string *_parName);
   string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
};

//////////////////////////// class RDOPMDWatchState /////////////////////////////////
class RDOPMDWatchState: public RDOPMDPokaz
{
	RDOCalc *logicCalc;

	int watchNumber;
	bool currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;
public:
	RDOPMDWatchState(string *_name, bool _trace, RDOFUNLogic *_logic);
   string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
};

//////////////////////////// class RDOPMDWatchQuant /////////////////////////////////
class RDOPMDWatchQuant: public RDOPMDPokaz
{
	RDOFUNGroup *funGroup;
	RDOCalc *logicCalc;

	int watchNumber;
	int currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;
public:
	RDOPMDWatchQuant(string *_name, bool _trace, string *_resTypeName);
	void setLogic(RDOFUNLogic *_logic);
	void setLogicNoCheck();
   string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
};

//////////////////////////// class RDOPMDWatchValue /////////////////////////////////
class RDOPMDWatchValue: public RDOPMDPokaz
{
	RDOFUNGroup *funGroup;
	RDOCalc *logicCalc;
	RDOCalc *arithmCalc;

	int watchNumber;
	double currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;
public:
	RDOPMDWatchValue(string *_name, bool _trace, string *_resTypeName);
	void setLogic(RDOFUNLogic *_logic, RDOFUNArithm *_arithm);
	void setLogicNoCheck(RDOFUNArithm *_arithm);
   string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(RDOResource *res);
};

//////////////////////////// class RDOPMDGetValue /////////////////////////////////
class RDOPMDGetValue: public RDOPMDPokaz
{
	RDOCalc *arithmCalc;
public:
	RDOPMDGetValue(string *_name, RDOFUNArithm *_arithm);
   string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
};


}		// namespace rdoParse 

#endif //RDOPMD_PMD
