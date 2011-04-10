/*
 * copyright: (c) RDO-Team, 2011
 * filename : info.h
 * author   : Урусов Андрей
 * date     : 09.04.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_INFO_H_
#define _RDOPARSER_TYPE_INFO_H_

// ====================================================================== INCLUDES
#include <boost/optional.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------
OBJECT(TypeInfo)
{
DECLARE_FACTORY(TypeInfo)
public:
	CREF(LPRDOType)        type    () const;
	CREF(RDOParserSrcInfo) src_info(CREF(RDOParserSrcInfo) srcInfo) const;

private:
	TypeInfo(CREF(LPTypeInfo) pTypeInfo);
	TypeInfo(CREF(LPRDOType)  pType, CREF(RDOParserSrcInfo) srcInfo);
	virtual ~TypeInfo();

	LPRDOType                         m_pType;
	boost::optional<RDOParserSrcInfo> m_srcInfo;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/type/info.inl"

#endif //! _RDOPARSER_TYPE_INFO_H_
