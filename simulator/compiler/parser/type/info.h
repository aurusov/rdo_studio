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
#include "simulator/compiler/parser/type/type.h"
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
	const LPRDOType&        type      () const;
	RDOParserSrcInfo        src_info  () const;
	const RDOParserSrcInfo& src_info  (const RDOParserSrcInfo& srcInfo) const;
	LPTypeInfo              type_cast (const LPTypeInfo& pFrom, const RDOParserSrcInfo& src_info) const;
	LPRDOValue              value_cast(const LPRDOValue& pValue) const;

protected:
	TypeInfo(const LPTypeInfo& pTypeInfo);
	virtual ~TypeInfo();

private:
	TypeInfo(const LPRDOType& pType, const RDOParserSrcInfo& srcInfo);

	template <class T>
	static LPTypeInfo create(const RDOParserSrcInfo& srcInfo);

	LPRDOType                         m_pType;
	boost::optional<RDOParserSrcInfo> m_srcInfo;

	void init();
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/type/info.inl"

#endif // _RDOPARSER_TYPE_INFO_H_
