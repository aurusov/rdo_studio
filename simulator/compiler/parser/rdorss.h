/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Романов Ярослав (robot.xet@gmail.com)
  \date
  \brief
  \indent    4T
*/

#ifndef _RDORSS_RSS_H_
#define _RDORSS_RSS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_as_string_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
class RDORSSResource
	: public RDOParserSrcInfo
	, public boost::noncopyable
	, public Context
	, public IContextFind
	, public rdo::runtime::IAsString
{
DECLARE_FACTORY(RDORSSResource);
public:
	static const std::string GET_RESOURCE;

	class Param
	{
	public:
		explicit Param(const LPExpression& pValue)
			: m_pValue(pValue)
		{}

		const LPExpression& param() const
		{
			return m_pValue;
		}

		LPExpression& param()
		{
			return m_pValue;
		}

	private:
		LPExpression m_pValue;
	};
	typedef std::vector<Param> ParamList;
	static const std::size_t UNDEFINED_ID = std::size_t(~0);

	virtual rdo::runtime::LPRDOCalc createCalc() const;
	virtual std::vector<rdo::runtime::LPRDOCalc> createCalcList() const;

	const std::string& name() const { return src_info().src_text(); }
	LPRDORTPResType getType() const { return m_pResType; }

	std::size_t getID() const { return m_id; }

	const ParamList& params () const { return m_paramList; }

	void addParam(const LPRDOValue& pParam);
	bool getTrace() const { return trace; }
	void setTrace(bool value) { trace = value; }
	bool defined() const;
	void end();
	void setIsNested(bool nested) { isNested = nested; }
	bool getIsNested() { return isNested; }

	void writeModelStructure(std::ostream& stream) const;

	LPExpression  createGetResourceExpression(const RDOParserSrcInfo& srcInfo) const;

	void setTraceCalc (const rdo::runtime::LPRDOCalc& pTraceCalc)
	{
		m_traceCalc = pTraceCalc;
	}

	DECLARE_IAsString;

protected:
	RDORSSResource(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id = UNDEFINED_ID);
	virtual ~RDORSSResource();

	LPRDORTPResType m_pResType;
	std::size_t m_id; //! in system
	ParamList m_paramList;
	bool trace;
	bool isNested;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;
	rdo::runtime::LPRDOCalc m_traceCalc;

	virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDORSSResource);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORSS_RSS_H_
