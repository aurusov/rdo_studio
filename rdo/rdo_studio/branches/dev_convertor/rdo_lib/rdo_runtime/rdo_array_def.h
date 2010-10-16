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
PREDECLARE_POINTER(RDOArrayType);

class RDOArrayValue
{
public:
	typedef std::vector<CPTR(RDOValue)> ArrayValue;

	RDOArrayValue(CREF(LPRDOArrayType) pType);
	RDOArrayValue(CREF(RDOArrayValue)  value);
	~RDOArrayValue();

	CREF(LPRDOArrayType) type() const;

private:
	ArrayValue      m_arrayValue;
	LPRDOArrayType  m_pArrayType;
};

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	typedef LPRDOType LPArrayType;

	//virtual tstring  asString()                    const;
	virtual RDOValue cast    (CREF(RDOValue) from) const;

private:
	RDOArrayType();
	RDOArrayType(CREF(LPArrayType) pArrayType);

	LPArrayType m_pArrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ARRAY_DEF_H_
