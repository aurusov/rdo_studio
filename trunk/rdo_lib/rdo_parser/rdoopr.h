#ifndef RDOOPR_H
#define RDOOPR_H

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

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
class RDOPATPattern;

class RDOOPROperation: public RDOParserObject, public RDOParserSrcInfo
{
private:
	const RDOPATPattern*            pattern;
	int                             currParam;
	rdoRuntime::RDOActivityRuntime* activity;

public:
	RDOOPROperation( RDOParser* _parser, const RDOParserSrcInfo& _name, const RDOParserSrcInfo& _pattern );
	void addParam( const std::string& stringParam, const YYLTYPE& param_pos );
	void addParam( int intParam, const YYLTYPE& param_pos );
	void addParam( double realParam, const YYLTYPE& param_pos );
	void addParam( const YYLTYPE& param_pos );
	void addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos );
	void end( const YYLTYPE& opr_pos );
	const std::string& getName() const { return src_info().src_text(); }
	const RDOPATPattern* getType() const { return pattern; }
};

} // namespace rdoParse 

#endif // RDOOPR_H
