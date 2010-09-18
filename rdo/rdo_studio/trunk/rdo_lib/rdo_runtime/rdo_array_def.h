/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array_def.h
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */
#ifndef _RDO_ARRAY_DEF_H_
#define _RDO_ARRAY_DEF_H_

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (вместо "rdo_array_def.h" надо инклюдить "rdo_array.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "rdo_array.h"
//

// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayType;

class RDOArrayValue
{
public:
	typedef std::vector<CPTR(RDOValue)> ArrayValue;

	RDOArrayValue(CREF(RDOArrayType)  type );
	RDOArrayValue(CREF(RDOArrayValue) value);
	~RDOArrayValue();

	CREF(RDOArrayType) type() const;

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

	RDOArrayType(PTR(RDORuntimeParent) parent);
	RDOArrayType(PTR(RDORuntimeParent) parent,CREF(ArrayType) arraytype);

	ArrayType getArrayType() const;
	//virtual tstring  asString()                    const;
	virtual RDOValue cast    (CREF(RDOValue) from) const;

private:
	ArrayType m_arrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ARRAY_DEF_H_
