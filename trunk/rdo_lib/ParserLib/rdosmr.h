#ifndef RDOSMR_SMR
#define RDOSMR_SMR

#include "rdoStdFuncs.h"
#include "rdotrace.h"
#include <rdocommon.h>

namespace rdoRuntime
{
class RDOCalc;
}

using namespace rdoRuntime;


namespace rdoParse 
{
struct RDOFUNLogic;
struct RDOFUNArithm;

class BreakPoint: public RDODeletable
{
	string *name;
	RDOFUNLogic *logic;
};

class RDOSMR: public RDODeletable
{
public:
	string *modelName;
	string *resourceFileName;
	string *oprIevFileName;
	string *frameFileName;
	string *statisticFileName;
	string *resultsFileName;
	string *traceFileName;

	RDOSimulatorNS::ShowMode showMode;

	bool showModeSet;
	bool frameNumberSet;

	int frameNumber;
	double *showRate;
	double *runStartTime;
	double *traceStartTime;
	double *traceEndTime;

	RDOFUNLogic *terminateIf;
	vector<BreakPoint *> breakPoints;
	RDOCalc *startCalcs;

public:
	RDOSMR(string *_modelName);
	void setValue(const char* descrName, string* RDOSMR::*pMem, string* newValue);
	void setValue(const char* descrName, double* RDOSMR::*pMem, double* newValue);

	void setShowMode(RDOSimulatorNS::ShowMode sm);
	void setFrameNumber(int fn);
	void setTerminateIf(RDOFUNLogic *logic);
	void setConstValue(string *constName, RDOFUNArithm *arithm);
	void setResParValue(string *resName, string *parName, RDOFUNArithm *arithm);
	void setSeed(string *seqName, int _base);
};

}		// namespace rdoParse 

#endif //RDOSMR_SMR
