/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDORTP_H_
#define _RDORTP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/type/type.h"
#include "rdo_lib/rdo_parser/type/enum.h"
#include "rdo_lib/rdo_parser/rdortp_param.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  rtpparse(PTR(void) lexer);
int  rtplex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rtperror(PTR(char) mes);

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);
PREDECLARE_POINTER(RDORSSResource);

OBJECT(RDORTPResType) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDORTPResType);
public:
	typedef std::vector<LPRDORTPParam> ParamList;

	enum { UNDEFINED_PARAM = ~0 };

	CREF(tstring) name       () const   { return src_text();   };
	rsint         getNumber  () const   { return m_number;     };
	rbool         isPermanent() const   { return m_permanent;  };
	rbool         isTemporary() const   { return !m_permanent; };

	LPRDORSSResource createRes(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info);

	void addParam(CREF(LPRDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(CREF(tstring) paramName) const;

	ruint           getRTPParamNumber(CREF(tstring) paramName) const;
	CREF(ParamList) getParams        ()                        const { return m_params;          }

	CREF(rdoRuntime::LPIResourceType) getRuntimeResType() const      { return m_pRuntimeResType; }

	template<class T>
	void end()
	{
		m_pRuntimeResType = rdo::Factory<T>::create(m_number).interface_cast<rdoRuntime::IResourceType>();
		ASSERT(m_pRuntimeResType);
	}

	void writeModelStructure(REF(std::ostream) stream) const;

private:
	RDORTPResType(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent);
	virtual ~RDORTPResType();

	rdoRuntime::LPIResourceType m_pRuntimeResType;
	const ruint                 m_number;
	const rbool                 m_permanent;
	ParamList                   m_params;
};
DECLARE_POINTER(RDORTPResType);

//// --------------------------------------------------------------------------------
////------------------------------ FOR FUZZY LOGIC ------------------------------	
//// --------------------------------------------------------------------------------
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyMembershiftPoint - точка ф-ии принадлежности нечеткого терма
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyMembershiftPoint: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyMembershiftPoint(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, double x_value, double y_value):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_x_value(x_value),
//		m_y_value(y_value)
//		{
//		}
//	virtual ~RDORTPFuzzyMembershiftPoint() {}
//
//	double  getX() const { return m_x_value; }
//	double  getY() const { return m_y_value; }
//
//private:
//	double    m_x_value;
//	double    m_y_value;
//};
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности для нечеткого терма
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyMembershiftFun: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyMembershiftFun(CREF(LPRDOParser) pParser):
//		RDOParserObject(pParser)
//		{
//		}
//	virtual ~RDORTPFuzzyMembershiftFun() {}
//
//	typedef PTR(RDORTPFuzzyMembershiftPoint) Item;
//	typedef std::vector< Item >              Items;
//
//	void add(Item point)
//	{
//		m_points.push_back(point);
//	}
//	double  getVal() const 
//	{ 
//		return m_value; 
//	}
//
//private:
//	Items m_points;	// точки, определяющие ф-ию принадлежности
//	double m_value;	// значение ф-ии принадлежности для конкретного четкого значения
//};
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyTerm - нечеткий термин
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyTerm: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyTerm(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_fun (pMembersfift_fun)
//		{
//		}
//	virtual ~RDORTPFuzzyTerm() {}
//
//	CREF(tstring) name       () const { return src_info().src_text(); }
//	double        MemberShift() const { return m_fun->getVal();       }
//
//private:
//	PTR(RDORTPFuzzyMembershiftFun) m_fun;
//};
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyTermsSet - набор терминов одного параметра
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyTermsSet(CREF(LPRDOParser) pParser)
//		: RDOParserObject(pParser)
//	{
//	}
//	virtual ~RDORTPFuzzyTermsSet() {}
//
//	typedef PTR(RDORTPFuzzyTerm) Item;
//	typedef std::vector<Item>    Items;
//
//	void add(Item term)
//	{
//		m_terms.push_back(term);
//	}
//	rbool empty() const
//	{
//		return m_terms.empty();
//	}
//
//private:
//	Items m_terms; // набор терминов одного параметра
//};
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyParam
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyParam : public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyParam(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyTermsSet) terms_set):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_set (terms_set)
//		{
//		}
//	virtual ~RDORTPFuzzyParam() {}
//
//	CREF(tstring) name() const { return src_info().src_text(); }
//
//private:
//	PTR(RDORTPFuzzyTermsSet) m_set; // набор терминов параметра
//};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORTP_H_
