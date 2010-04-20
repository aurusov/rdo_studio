#ifndef RDORSS_RSS
#define RDORSS_RSS

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse
{

int  rssparse(PTR(void) lexer);
int  rsslex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rsserror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
class RDORTPResType;
class RDORTPParam;

class RDORSSResource: public RDOParserObject, public RDOParserSrcInfo
{
public:
	class Param
	{
	public:
		explicit Param(CREF(RDOValue) value)
			: m_value(value)
		{}

		CREF(RDOValue) param() const
		{
			return m_value;
		}

	private:
		RDOValue m_value;
	};
	typedef std::vector<Param> Params;
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
	RDORTPResType::ParamList::const_iterator m_currParam;
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
