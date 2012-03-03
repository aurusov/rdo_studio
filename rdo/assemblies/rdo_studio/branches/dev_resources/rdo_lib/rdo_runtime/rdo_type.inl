
#include "rdo_exception.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
inline RDOType::RDOType(TypeID typeID)
	: m_typeID(typeID)
{}

inline RDOType::TypeID RDOType::typeID() const { return m_typeID; }

inline rbool RDOType::operator!= (CREF(RDOType) type)
{
	return m_typeID != type.m_typeID;
}

} // namespace rdoRuntime
