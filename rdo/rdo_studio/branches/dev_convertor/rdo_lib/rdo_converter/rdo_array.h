/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_ARRAY_H_
#define _CONVERTOR_RDOCONVERTER_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	CREF(rdoRuntime::RDOArrayType) getArray() const { return *static_cast<CPTR(rdoRuntime::RDOArrayType)>(m_type); }

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOArrayType         ();
	virtual ~RDOArrayType();

	PTR(rdoRuntime::RDOArrayType) __array() const { return static_cast<PTR(rdoRuntime::RDOArrayType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type)); }
};
DECLARE_POINTER(RDOArrayType);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_ARRAY_H_
