#ifndef RDORSS_RSS
#define RDORSS_RSS

#include "rdoparser_object.h"
#include "rdoparser_value.h"
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
public:
	typedef std::vector< rdoRuntime::RDOValue > Params;
	enum { UNDEFINED_ID = ~0 };

	RDORSSResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType, int id = UNDEFINED_ID );

	virtual rdoRuntime::RDOCalc* createCalc();

	const std::string&         name() const    { return src_info().src_text(); }
	const RDORTPResType* const getType() const { return resType;  }

	int getID() const                          { return m_id;     }

	const Params& params() const { return m_params;  }
	void addParam( const RDOValue& param );

	void writeModelStructure( std::ostream& stream ) const;

	bool getTrace() const       { return trace;  }
	void setTrace( bool value ) { trace = value; }
	
	bool defined() const;

protected:
	const RDORTPResType* const resType;
	const int                  m_id;        // in system
	Params                     m_params;
	bool                       trace;

private:
	std::vector< const RDORTPParam* >::const_iterator m_currParam;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
public:
	RDOPROCResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType, int id = UNDEFINED_ID );

private:
	virtual rdoRuntime::RDOCalc* createCalc();
};

} // namespace rdoParse

#endif // RDORSS_RSS
