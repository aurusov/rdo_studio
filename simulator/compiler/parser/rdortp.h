#ifndef _RDORTP_H_
#define _RDORTP_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);
PREDECLARE_POINTER(RDORSSResource);

class RDORTPResType
	: public RDOParserSrcInfo
	, public boost::noncopyable
	, public rdo::runtime::RDOResourceTypeList
	, public IType
	, public IModelStructure
	, public Context
	, public IContextFind
	, public virtual rdo::counter_reference
{
DECLARE_FACTORY(RDORTPResType);
public:
	typedef std::vector<LPRDORTPParam> ParamList;
	typedef RDORSSResource value_type;

	enum Subtype
	{
		RT_SIMPLE,
		RT_PROCESS_RESOURCE,
		RT_PROCESS_TRANSACT
	};
	static const std::size_t UNDEFINED_PARAM = std::size_t(~0);

	int getNumber() const;
	bool isPermanent() const;
	bool isTemporary() const;

	LPRDORSSResource createRes(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info);

	void addParam(const LPRDORTPParam& param);
	void addParam(const std::string& param_name, rdo::runtime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(const std::string& paramName) const;

	std::size_t getRTPParamNumber(const std::string& paramName) const;
	const ParamList& getParams() const;

	void setSubtype(Subtype subtype);

	void setupRuntimeFactory();

	void writeModelStructure(std::ostream& stream) const;

	DECLARE_IType;

private:
	RDORTPResType(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, bool permanent);
	virtual ~RDORTPResType();

	const std::size_t m_number;
	const bool m_permanent;
	boost::optional<Subtype> m_subtype;
	ParamList m_params;

	virtual runtime::RDOType::TypeID typeID() const;
	virtual LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
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
//	RDORTPFuzzyMembershiftPoint(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, double x_value, double y_value):
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
//	RDORTPFuzzyMembershiftFun(const LPRDOParser& pParser):
//		RDOParserObject(pParser)
//		{
//		}
//	virtual ~RDORTPFuzzyMembershiftFun() {}
//
//	typedef RDORTPFuzzyMembershiftPoint* Item;
//	typedef std::vector<Item> Items;
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
//	RDORTPFuzzyTerm(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, RDORTPFuzzyMembershiftFun* pMembersfift_fun):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_fun (pMembersfift_fun)
//		{
//		}
//	virtual ~RDORTPFuzzyTerm() {}
//
//	const std::string& name () const { return src_info().src_text(); }
//	double MemberShift() const { return m_fun->getVal(); }
//
//private:
//	RDORTPFuzzyMembershiftFun* m_fun;
//};
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyTermsSet - набор терминов одного параметра
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyTermsSet(const LPRDOParser& pParser)
//		: RDOParserObject(pParser)
//	{
//	}
//	virtual ~RDORTPFuzzyTermsSet() {}
//
//	typedef RDORTPFuzzyTerm* Item;
//	typedef std::vector<Item> Items;
//
//	void add(Item term)
//	{
//		m_terms.push_back(term);
//	}
//	bool empty() const
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
//	RDORTPFuzzyParam(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, RDORTPFuzzyTermsSet* terms_set):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_set (terms_set)
//		{
//		}
//	virtual ~RDORTPFuzzyParam() {}
//
//	const std::string& name() const { return src_info().src_text(); }
//
//private:
//	RDORTPFuzzyTermsSet* m_set; // набор терминов параметра
//};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORTP_H_
