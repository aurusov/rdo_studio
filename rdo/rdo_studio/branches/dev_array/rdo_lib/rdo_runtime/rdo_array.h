#ifndef RDO_ARRAY_H
#define RDO_ARRAY_H

#include "rdo_type.h"
#include "rdoruntime_object.h"
#include <rdocommon.h>
#include <string>

namespace rdoRuntime 
{

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

} // namespace rdoRuntime

#include "rdo_array.inl"

#endif // RDO_ARRAY_H
