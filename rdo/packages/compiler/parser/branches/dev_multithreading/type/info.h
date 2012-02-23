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
OBJECT(TypeInfo)
{
DECLARE_FACTORY(TypeInfo)
public:
	CREF(LPRDOType)        type      () const;
	RDOParserSrcInfo       src_info  () const;
	CREF(RDOParserSrcInfo) src_info  (CREF(RDOParserSrcInfo) srcInfo) const;
	LPTypeInfo             type_cast (CREF(LPTypeInfo) pFrom, CREF(RDOParserSrcInfo) src_info) const;
	LPRDOValue             value_cast(CREF(LPRDOValue) pValue) const;

protected:
	TypeInfo(CREF(LPTypeInfo) pTypeInfo);
	virtual ~TypeInfo();

private:
	TypeInfo(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) srcInfo);

	LPRDOType                         m_pType;
	boost::optional<RDOParserSrcInfo> m_srcInfo;

	void init();
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/type/info.inl"

#endif // _RDOPARSER_TYPE_INFO_H_
