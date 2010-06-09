/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : Чирков Михаил
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
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayValue
{
	DECLARE_FACTORY(RDOArrayValue);
private:
	typedef std::vector<CPTR(RDOValue)> ArrayValue;

	RDOArrayValue         ();
	RDOArrayValue         (CREF(RDOArrayValue) pValue);
	virtual ~RDOArrayValue();

	//DECLARE_IType;
	//DECLARE_IModelStructure;
	ArrayValue         m_arrayValue;
public:
	void insert_array(CREF(RDOValue) pArray);
};
DECLARE_POINTER(RDOArrayValue);

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public rdo::smart_ptr_counter_reference
{
DECLARE_FACTORY(RDOArrayType);
public:
	CREF(LPRDOType)                   type() const;
private:
	RDOArrayType         (CREF(LPRDOType) pType);
	virtual ~RDOArrayType();

	PTR(rdoRuntime::RDOArrayType) __array() const 
	{ 
		return static_cast<PTR(rdoRuntime::RDOArrayType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type));
	}

	LPRDOType m_pType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ARRAY_H_