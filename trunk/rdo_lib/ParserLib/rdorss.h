#ifndef RDORSS_RSS
#define RDORSS_RSS

#include "rdoStdFuncs.h"

namespace rdoParse 
{

int rssparse( void* lexer );
int rsslex( int* lpval, void* lexer );
void rsserror( char* mes );

class RDORTPResType;
class RDORTPParamDesc;

class RDORSSResource: public RDODeletable
{
	const std::string *const name;
	const RDORTPResType *const resType;
	const int number;		// in system
	std::vector<RDOValue> values;

public:
	std::vector<const RDORTPParamDesc *>::const_iterator currParam;

public:
	RDORSSResource(const std::string *const _name, const RDORTPResType *const _resType, const int _number); 
	const RDORTPResType *const getType() const { return resType; }
	int getNumber() const { return number; }
	const std::string *const getName() const { return name; }
	const std::vector<RDOValue>& getValues() const { return values; }
	void addValue(RDOValue val) { values.push_back(val); }
	int writeModelStructure();
};


}		// namespace rdoParse 

#endif //RDORSS_RSS
