/*!
  \copyright (c) RDO-Team, 2011
  \file      info.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_INFO_H_
#define _RDOPARSER_TYPE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeInfo
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(TypeInfo);
class TypeInfo: public rdo::counter_reference
{
DECLARE_FACTORY(TypeInfo)
public:
	rdo::runtime::LPRDOType type() const;
	rdo::runtime::RDOType::TypeID typeID() const;
	const LPIType& itype() const;
	RDOParserSrcInfo src_info() const;
	const RDOParserSrcInfo& src_info(const RDOParserSrcInfo& srcInfo) const;
	LPTypeInfo type_cast(const LPTypeInfo& pFrom, const RDOParserSrcInfo& src_info) const;
	LPRDOValue value_cast(const LPRDOValue& pValue) const;

protected:
	TypeInfo(const LPTypeInfo& pTypeInfo);
	virtual ~TypeInfo();

private:
	TypeInfo(const LPIType& pType, const RDOParserSrcInfo& srcInfo);

	template <class T>
	static LPTypeInfo create(const RDOParserSrcInfo& srcInfo);

	const LPIType m_pType;
	boost::optional<RDOParserSrcInfo> m_srcInfo;

	void init();
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/type/info-inl.h"

#endif // _RDOPARSER_TYPE_INFO_H_
