/*
 * copyright: (c) RDO-Team, 2010
 * filename : array.h
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_ARRAY_H_
#define _RDOPARSER_TYPE_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
public:
	CREF(LPRDOType)            getItemType        () const;
	rdoRuntime::LPRDOArrayType getRuntimeArrayType() const;

private:
	RDOArrayType         (CREF(LPRDOType) pItemType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOArrayType();

	LPRDOType m_pItemType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_ARRAY_H_
