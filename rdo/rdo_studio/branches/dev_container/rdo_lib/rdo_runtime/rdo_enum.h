#ifndef RDO_ENUM_H
#define RDO_ENUM_H

#include "rdo_type.h"
#include "rdoruntime_object.h"
#include <rdocommon.h>
#include <string>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
class RDOEnumType: public RDOType, public RDORuntimeObject
{
public:
	typedef tstring                  EnumItem;
	typedef rdo::vector< EnumItem >  Enums;
	typedef Enums::const_iterator    CIterator;

	enum { END = ~0 };

	RDOEnumType(PTR(RDORuntimeParent) parent);
	RDOEnumType(PTR(RDORuntimeParent) parent, CREF(Enums) enums);

	void  add     (CREF(tstring) next);
	ruint findEnum(CREF(tstring) val) const;
	rbool exist   (CREF(tstring) val) const;
	virtual tstring  asString()                    const;
	virtual RDOValue cast    (CREF(RDOValue) from) const;

	rbool           empty    () const;
	const CIterator begin    () const;
	const CIterator end      () const;
	CREF(Enums)     getValues() const;

private:
	Enums m_enum;

	tstring str() const;
};

} // namespace rdoRuntime

#include "rdo_enum.inl"

#endif // RDO_ENUM_H
