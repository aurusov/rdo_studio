#ifndef RDOSMR_SMR
#define RDOSMR_SMR

#include "rdogramma.h"
#include "rdoStdFuncs.h"
#include <rdotrace.h>
#include <rdocommon.h>

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse 
{

int smr1parse( void* lexer );
int smr1lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr1error( char* mes );

int smr2parse( void* lexer );
int smr2lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr2error( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

class BreakPoint: public RDODeletable
{
	std::string *name;
	RDOFUNLogic *logic;
};

class RDOSMR: public RDODeletable
{
public:
	std::string *modelName;
	std::string *resourceFileName;
	std::string *oprIevFileName;
	std::string *frameFileName;
	std::string *statisticFileName;
	std::string *resultsFileName;
	std::string *traceFileName;

	rdosim::ShowMode showMode;

	bool showModeSet;
	bool frameNumberSet;

	int frameNumber;
	double *showRate;
	double *runStartTime;
	double *traceStartTime;
	double *traceEndTime;

	RDOFUNLogic *terminateIf;
	std::vector<BreakPoint *> breakPoints;
	RDOCalc *startCalcs;

public:
	RDOSMR(std::string *_modelName);
	void setValue(const char* descrName, std::string* RDOSMR::*pMem, std::string* newValue);
	void setValue(const char* descrName, double* RDOSMR::*pMem, double* newValue);

	void setShowMode(rdosim::ShowMode sm);
	void setFrameNumber(int fn);
	void setTerminateIf(RDOFUNLogic *logic);
	void setConstValue(std::string *constName, RDOFUNArithm *arithm);
	void setResParValue(std::string *resName, std::string *parName, RDOFUNArithm *arithm);
	void setSeed(std::string *seqName, int _base);
};

}		// namespace rdoParse 

#endif //RDOSMR_SMR
