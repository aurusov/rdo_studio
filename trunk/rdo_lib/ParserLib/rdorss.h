#ifndef RDORSS_RSS
#define RDORSS_RSS

using namespace std;
#include "rdoStdFuncs.h"

namespace rdoParse 
{

class RDORTPResType;
class RDORTPParamDesc;

class RDORSSResource: public RDODeletable
{
	const string *const name;
	const RDORTPResType *const resType;
	const int number;		// in system
	vector<RDOValue> values;

public:
	vector<const RDORTPParamDesc *>::const_iterator currParam;

public:
	RDORSSResource(const string *const _name, const RDORTPResType *const _resType, const int _number); 
	const RDORTPResType *const getType() const { return resType; }
	int getNumber() const { return number; }
	const string *const getName() const { return name; }
	const vector<RDOValue>& getValues() const { return values; }
	void addValue(RDOValue val) { values.push_back(val); }
	int writeModelStructure();
};


}		// namespace rdoParse 

#endif //RDORSS_RSS
