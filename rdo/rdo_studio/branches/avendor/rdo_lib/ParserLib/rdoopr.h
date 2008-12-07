#ifndef RDOOPR_OPR
#define RDOOPR_OPR

#include "rdoStdFuncs.h"


namespace rdoRuntime
{
class RDOActivityRuntime;
}

using namespace rdoRuntime;

namespace rdoParse 
{

class RDOPATPattern;

class RDOOPROperation: public RDODeletable
{
	const string *const name;
	const RDOPATPattern *pattern;
	int currParam;
	RDOActivityRuntime *activity;
public:
	RDOOPROperation(string *_oprName, string *patName);
	void endOfDefinition();
	void addParam(string *stringParam);
	void addParam(int intParam);
	void addParam(double *realParam);
	void addParam();
	const string *const getName() const { return name; }
	void addHotKey(string *hotKey);
};


}		// namespace rdoParse 

#endif //RDOOPR_OPR
