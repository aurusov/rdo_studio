/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : Чирков Михаил, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_ARRAY_H_
#define _RDOPARSER_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public rdo::smart_ptr_counter_reference, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
friend class RDOArrayValue;

public:
	CREF(LPRDOType) itemType() const;

private:
	RDOArrayType         (CREF(LPRDOType) pItemType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOArrayType();

	PTR(rdoRuntime::RDOArrayType) __array() const 
	{ 
		return static_cast<PTR(rdoRuntime::RDOArrayType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type));
	}

	LPRDOType m_pItemType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayValue
{
DECLARE_FACTORY(RDOArrayValue);
public:
	void                 insertItem  (CREF(RDOValue) value);
	CREF(LPRDOArrayType) getArrayType() const;
	rdoRuntime::RDOValue getRValue   () const;

private:
	typedef std::vector<RDOValue> Container;

	RDOArrayValue         (LPRDOArrayType pArrayType);
	virtual ~RDOArrayValue();

	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};
DECLARE_POINTER(RDOArrayValue);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ARRAY_H_