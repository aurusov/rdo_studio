/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDORTP_H_
#define _CONVERTOR_RDORTP_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/rdo_enum.h"
#include "converter/smr2rdox/rdortp_param.h"

#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_rtpparse(PTR(void) lexer);
int  cnv_rtplex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_rtperror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
class Converter;

OBJECT(RDORTPResType)
	IS  INSTANCE_OF(RDOParserSrcInfo  )
	AND INSTANCE_OF(boost::noncopyable)
{
DECLARE_FACTORY(RDORTPResType);
public:
	typedef std::vector<LPRDORTPParam> ParamList;

	static const ruint UNDEFINED_PARAM = ruint(~0);

	CREF(tstring) name       () const   { return src_text();   };
	rsint         getNumber  () const   { return m_number;     };
	rbool         isPermanent() const   { return m_permanent;  };
	rbool         isTemporary() const   { return !m_permanent; };

	void addParam(CREF(LPRDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdo::runtime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(CREF(tstring) paramName) const;
	void finish  ();

	ruint           getRTPParamNumber(CREF(tstring) paramName) const;
	CREF(ParamList) getParams        ()                        const { return m_params; }

	void writeModelStructure(REF(std::ostream) stream) const;

private:
	RDORTPResType(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent);
	virtual ~RDORTPResType();

	const rsint m_number;
	const rbool m_permanent;
	ParamList   m_params;
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
//	RDORTPFuzzyMembershiftPoint(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, double x_value, double y_value):
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
//	RDORTPFuzzyMembershiftFun(PTR(Converter) pParser):
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
//	RDORTPFuzzyTerm(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
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
//	RDORTPFuzzyTermsSet(PTR(Converter) pParser)
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
//	RDORTPFuzzyParam(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyTermsSet) terms_set):
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDORTP_H_
