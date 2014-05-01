/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/type/array.h
  \author    Чирков Михаил
  \date
  \brief
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ARRAY_H_
#define _RDOPARSER_TYPE_ARRAY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_array.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/type/runtime_wrapper_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayType
// --------------------------------------------------------------------------------
class RDOArrayType
	: public rdo::runtime::RDOArrayType
	, public IType
	, public IModelStructure
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
public:
	const LPTypeInfo& getItemType() const;
	LPTypeInfo typeInfo() const;

private:
	RDOArrayType(const LPTypeInfo& pItemType, const RDOParserSrcInfo& src_info);
	virtual ~RDOArrayType();

	LPTypeInfo m_pItemType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ARRAY_H_
