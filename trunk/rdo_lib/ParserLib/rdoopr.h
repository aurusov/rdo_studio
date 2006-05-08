#ifndef RDOOPR_OPR
#define RDOOPR_OPR

#include "rdogramma.h"
#include "rdoStdFuncs.h"

namespace rdoRuntime
{
class RDOActivityRuntime;
}

using namespace rdoRuntime;

namespace rdoParse 
{

int oprparse( void* lexer );
int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void oprerror( char* mes );

class RDOPATPattern;

class RDOOPROperation: public RDODeletable
{
	const std::string *const name;
	const RDOPATPattern *pattern;
	int currParam;
	RDOActivityRuntime *activity;
public:
	RDOOPROperation(std::string *_oprName, std::string *patName);
	void endOfDefinition();
	void addParam(std::string *stringParam);
	void addParam(int intParam);
	void addParam(double *realParam);
	void addParam();
	const std::string *const getName() const { return name; }
	void addHotKey(std::string *hotKey);
};


}		// namespace rdoParse 

#endif //RDOOPR_OPR
