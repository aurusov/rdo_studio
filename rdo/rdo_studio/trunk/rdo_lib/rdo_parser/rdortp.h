#ifndef RDORTP_H
#define RDORTP_H

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_enum.h"
#include "rdo_lib/rdo_parser/rdortp_param.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"

namespace rdoParse 
{

int  rtpparse(PTR(void) lexer);
int  rtplex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rtperror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	typedef std::vector<LPRDORTPParam> ParamList;

	enum { UNDEFINED_PARAM = ~0 };

	RDORTPResType(PTR(RDOParser) _parser, CREF(RDOParserSrcInfo) _src_info, rbool _permanent);
	virtual ~RDORTPResType();
	CREF(tstring) name       () const   { return src_text();   };
	rsint         getNumber  () const   { return m_number;     };
	rbool         isPermanent() const   { return m_permanent;  };
	rbool         isTemporary() const   { return !m_permanent; };

	void addParam(CREF(LPRDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(CREF(tstring) paramName) const;

	ruint           getRTPParamNumber(CREF(tstring) paramName) const;
	CREF(ParamList) getParams        ()                        const { return m_params; }

	void writeModelStructure(REF(std::ostream) stream) const;

private:
	const rsint  m_number;
	const rbool  m_permanent;
	ParamList    m_params;
};

// ----------------------------------------------------------------------------
//------------------------------ FOR FUZZY LOGIC ------------------------------	
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftPoint - точка ф-ии принадлежности нечеткого терма
// ----------------------------------------------------------------------------
class RDORTPFuzzyMembershiftPoint: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftPoint( RDOParser* _parser, const RDOParserSrcInfo& _src_info, double x_value, double y_value ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_x_value( x_value ),
		m_y_value( y_value )
		{
		}
	virtual ~RDORTPFuzzyMembershiftPoint() {}

	double  getX() const { return m_x_value; }
	double  getY() const { return m_y_value; }

private:
	double    m_x_value;
	double    m_y_value;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности для нечеткого терма
// ----------------------------------------------------------------------------
class RDORTPFuzzyMembershiftFun: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftFun( RDOParser* _parser ):
		RDOParserObject( _parser )
		{
		}
	virtual ~RDORTPFuzzyMembershiftFun() {}

	typedef RDORTPFuzzyMembershiftPoint* Item;
	typedef std::vector< Item >          Items;

	void add( Item point )
	{
		m_points.push_back( point );
	}
	double  getVal() const 
	{ 
		return m_value; 
	}

private:
	Items m_points;	// точки, определяющие ф-ию принадлежности
	double m_value;	// значение ф-ии принадлежности для конкретного четкого значения
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTerm - нечеткий термин
// ----------------------------------------------------------------------------
class RDORTPFuzzyTerm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyTerm( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyMembershiftFun* membersfift_fun):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_fun (membersfift_fun)
		{
		}
	virtual ~RDORTPFuzzyTerm() {}

	const	std::string& name()	const	{ return src_info().src_text(); }
	double  MemberShift()			const	{ return m_fun->getVal(); }

private:
	RDORTPFuzzyMembershiftFun* m_fun;
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTermsSet - набор терминов одного параметра
// ----------------------------------------------------------------------------
class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyTermsSet( RDOParser* _parser )
		: RDOParserObject( _parser )
	{
	}
	virtual ~RDORTPFuzzyTermsSet() {}

	typedef RDORTPFuzzyTerm*	Item;
	typedef std::vector< Item >	Items;

	void add( Item term )
	{
		m_terms.push_back( term );
	}
	rbool empty() const
	{
		return m_terms.empty();
	}

private:
	Items m_terms; // набор терминов одного параметра
};

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyParam
// ----------------------------------------------------------------------------
class RDORTPFuzzyParam : public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyTermsSet* terms_set ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_set (terms_set)
		{
		}
	virtual ~RDORTPFuzzyParam() {}

	const std::string&           name() const       { return src_info().src_text(); }

private:
	RDORTPFuzzyTermsSet* m_set; // набор терминов параметра
};

} // namespace rdoParse

#endif // RDORTP_H
