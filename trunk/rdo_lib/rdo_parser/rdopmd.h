#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdogramma.h"
#include "rdoStdFuncs.h"
#include <rdotrace.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOResource;
}

namespace rdoParse 
{

int pmdparse( void* lexer );
int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void pmderror( char* mes );

class RDOFUNLogic;
class RDOFUNGroup;
class RDOFUNArithm;

//////////////////////////// class RDOPMDPokaz /////////////////////////////////
class RDOPMDPokaz: public RDOPokazTrace
{
protected:
	const std::string const name;
	void endOfCreation();
public:
	RDOPMDPokaz(const std::string *const _name, bool _trace);
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(RDOResource *res) { return false; }
	virtual void writePokazStructure(std::ostream &stream) const = 0;
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
	RDOPMDWatchPar(std::string *_name, bool _trace, std::string *_resName, std::string *_parName);
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
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
	RDOPMDWatchState(std::string *_name, bool _trace, RDOFUNLogic *_logic);
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
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
	RDOPMDWatchQuant(std::string *_name, bool _trace, std::string *_resTypeName);
	void setLogic(RDOFUNLogic *_logic);
	void setLogicNoCheck();
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
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
	RDOPMDWatchValue(std::string *_name, bool _trace, std::string *_resTypeName);
	void setLogic(RDOFUNLogic *_logic, RDOFUNArithm *_arithm);
	void setLogicNoCheck(RDOFUNArithm *_arithm);
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(RDOResource *res);
	void writePokazStructure(std::ostream &stream) const;
};

//////////////////////////// class RDOPMDGetValue /////////////////////////////////
class RDOPMDGetValue: public RDOPMDPokaz
{
	RDOCalc *arithmCalc;
public:
	RDOPMDGetValue(std::string *_name, RDOFUNArithm *_arithm);
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};


}		// namespace rdoParse 

#endif //RDOPMD_PMD
