#ifndef RDORSS_RSS
#define RDORSS_RSS

#include "rdoparser_object.h"
#include <rdoruntime_object.h>

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse
{

int rssparse( void* lexer );
int rsslex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rsserror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
class RDORTPResType;
class RDORTPParam;

class RDORSSResource: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	const RDORTPResType* const resType;
	const int                  number;  // in system
	std::vector< rdoRuntime::RDOValue > values;
	bool trace;

public:
	std::vector< const RDORTPParam* >::const_iterator currParam;

public:
	RDORSSResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType );

	virtual rdoRuntime::RDOCalc* createCalc();

	const std::string&         getName() const { return src_info().src_text(); }
	const RDORTPResType* const getType() const { return resType; }

	int getNumber() const                      { return number;  }

	const std::vector< rdoRuntime::RDOValue >& getValues() const { return values;  }
	void addValue( const rdoRuntime::RDOValue& val )             { values.push_back( val ); }

	int writeModelStructure();

	bool getTrace() const       { return trace;  }
	void setTrace( bool value ) { trace = value; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
private:
	virtual rdoRuntime::RDOCalc* createCalc();

public:
	RDOPROCResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType );
};

} // namespace rdoParse

#endif // RDORSS_RSS
