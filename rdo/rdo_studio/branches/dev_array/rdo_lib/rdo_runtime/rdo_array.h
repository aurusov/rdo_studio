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
class RDOArrayType: public RDOType
	{
	public:
		typedef CPTR(RDOType)				ArrayType;

		RDOArrayType(ArrayType arrayID);
		ArrayType ArrayID();

	private:
		ArrayType m_arrayID;
	};
} // namespace rdoRuntime

//#include "rdo_array.inl"

//#include "rdo_array.inl"

#endif // RDO_ARRAY_H