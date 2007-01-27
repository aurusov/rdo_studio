#ifndef RDORSS_RSS
#define RDORSS_RSS

#include "rdoparsebase.h"

namespace rdoParse 
{

int rssparse( void* lexer );
int rsslex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rsserror( char* mes );

class RDORTPResType;
class RDORTPParamDesc;

class RDORSSResource: public RDODeletable
{
protected:
	const std::string* const   name;
	const RDORTPResType* const resType;
	const int                  number;  // in system
	std::vector< rdoRuntime::RDOValue > values;
	bool trace;

public:
	std::vector< const RDORTPParamDesc* >::const_iterator currParam;

public:
	RDORSSResource( const std::string* const _name, const RDORTPResType* const _resType );
	
	const std::string*   const getName() const       { return name;    }
	const RDORTPResType* const getType() const       { return resType; }
	
	int getNumber() const                            { return number;  }

	const std::vector< rdoRuntime::RDOValue >& getValues() const { return values;  }
	void addValue( const rdoRuntime::RDOValue& val )             { values.push_back( val ); }
	
	int writeModelStructure();

	bool getTrace() const       { return trace;  }
	void setTrace( bool value ) { trace = value; }
};

class RDORSSTransact: public RDORSSResource
{
public:
	RDORSSTransact( const std::string* const _name, const RDORTPResType* const _resType );
};

}		// namespace rdoParse 

#endif // RDORSS_RSS
