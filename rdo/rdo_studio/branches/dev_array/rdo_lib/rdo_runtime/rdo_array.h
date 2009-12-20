/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array.h
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */
#ifndef RDO_ARRAY_H
#define RDO_ARRAY_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_type.h"
#include "rdoruntime_object.h"
#include <rdocommon.h>
#include <string>
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDORuntimeObject
{
public:
	typedef CPTR(RDOType) ArrayType;

	RDOArrayType(PTR(RDORuntimeParent) parent);
	RDOArrayType(PTR(RDORuntimeParent) parent,CREF(ArrayType) arraytype);

	virtual tstring  asString()                    const;
	virtual RDOValue cast    (CREF(RDOValue) from) const;

private:
	ArrayType m_arrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_array.inl"

#endif // RDO_ARRAY_H
