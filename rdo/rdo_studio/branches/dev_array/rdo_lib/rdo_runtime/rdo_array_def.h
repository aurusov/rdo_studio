/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array_def.h
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */
#ifndef RDO_ARRAY_DEF_H
#define RDO_ARRAY_DEF_H

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (и вообще, вместо "rdo_fuzzy_def.h" лучше юзать "rdo_fuzzy.h")
#endif //_RDO_VALUE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_type.h"
#include "rdoruntime_object.h"
#include <rdocommon.h>
#include <string>
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayType;

class RDOArrayValue
{
public:
	typedef std::vector<CPTR(RDOValue)>           ArrayValue;

	RDOArrayValue(CREF(RDOArrayType)  type );
	RDOArrayValue(CREF(RDOArrayValue) value);
	~RDOArrayValue();

	CREF(RDOArrayType)          type() const;

private:
	ArrayValue         m_arrayValue;
	CPTR(RDOArrayType) m_arrayTape;
};
// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDORuntimeObject
{
public:
	typedef CPTR(RDOType) ArrayType;

	RDOArrayType(PTR(RDORuntimeParent) parent,CREF(ArrayType) arraytype);

	virtual tstring  asString()                    const;
	virtual RDOValue cast    (CREF(RDOValue) from) const;

private:
	ArrayType m_arrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_array.inl"

#endif // RDO_ARRAY_DEF_H
