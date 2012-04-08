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
#include "simulator/compiler/parser/type/info.h"
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayType
// --------------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
public:
	CREF(LPTypeInfo)             getItemType        () const;
	rdo::runtime::LPRDOArrayType getRuntimeArrayType() const;
	LPTypeInfo                   typeInfo           () const;

private:
	RDOArrayType         (CREF(LPTypeInfo) pItemType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOArrayType();

	LPTypeInfo m_pItemType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ARRAY_H_
