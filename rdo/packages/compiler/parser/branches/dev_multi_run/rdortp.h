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
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/type.h"
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_switch_i.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  rtpparse(PTR(void) lexer);
int  rtplex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rtperror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);
PREDECLARE_POINTER(RDORSSResource);

CLASS(RDORTPResType):
	    INSTANCE_OF      (RDOParserSrcInfo  )
	AND INSTANCE_OF      (boost::noncopyable)
	AND INSTANCE_OF      (RDOType           )
	AND INSTANCE_OF      (Context           )
	AND IMPLEMENTATION_OF(IContextSwitch    )
{
DECLARE_FACTORY(RDORTPResType);
public:
	typedef std::vector<LPRDORTPParam> ParamList;

	enum TypeRDOResType
	{
		simple,
		procRes,
		procTran
	};
	enum { UNDEFINED_PARAM = ~0 };

	rsint getNumber  () const;
	rbool isPermanent() const;
	rbool isTemporary() const;

	LPRDORSSResource createRes(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info);

	void addParam(CREF(LPRDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdo::runtime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(CREF(tstring) paramName) const;

	ruint           getRTPParamNumber(CREF(tstring) paramName) const;
	CREF(ParamList) getParams() const;

	CREF(rdo::runtime::LPIResourceType) getRuntimeResType() const;

	void setType(TypeRDOResType type);

	void end();

	void writeModelStructure(REF(std::ostream) stream) const;

	DECLARE_IType;

private:
	RDORTPResType(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent, TypeRDOResType type = simple);
	virtual ~RDORTPResType();

	rdo::runtime::LPIResourceType m_pRuntimeResType;
	TypeRDOResType                m_type;
	const ruint                   m_number;
	const rbool                   m_permanent;
	ParamList                     m_params;

	DECLARE_IContextSwitch;
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

#include "simulator/compiler/parser/rdortp.inl"

#endif // _RDORTP_H_
