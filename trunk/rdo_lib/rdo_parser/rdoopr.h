#ifndef RDOOPR_OPR
#define RDOOPR_OPR

#include "rdoparser_object.h"

namespace rdoRuntime
{
class RDOActivityRuntime;
}

namespace rdoParse 
{

int oprparse( void* lexer );
int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void oprerror( char* mes );

class RDOPATPattern;

class RDOOPROperation: public RDOParserObject
{
private:
	std::string          name;
	const RDOPATPattern* pattern;
	int currParam;
	rdoRuntime::RDOActivityRuntime* activity;

public:
	RDOOPROperation( RDOParser* _parser, const std::string& _oprName, const std::string& patName );
	void endOfDefinition();
	void addParam( const std::string& stringParam );
	void addParam( int intParam );
	void addParam( double* realParam );
	void addParam();
	const std::string& getName() const { return name; }
	void addHotKey( const std::string& hotKey );
};

} // namespace rdoParse 

#endif // RDOOPR_OPR
