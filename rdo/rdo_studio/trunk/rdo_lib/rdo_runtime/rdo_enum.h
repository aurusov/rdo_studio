/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_ENUM_H_
#define _RDO_ENUM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
class RDOEnumType: public RDOType, public RDORuntimeObject
{
public:
	typedef tstring                EnumItem;
	typedef rdo::vector<EnumItem>  Enums;
	typedef Enums::const_iterator  CIterator;
	typedef Enums::const_iterator  const_iterator;

	enum { END = ~0 };

	RDOEnumType(PTR(RDORuntimeParent) parent);
	RDOEnumType(PTR(RDORuntimeParent) parent, CREF(Enums) enums);

	void  add     (CREF(tstring) next);
	ruint findEnum(CREF(tstring) val) const;
	rbool exist   (CREF(tstring) val) const;
	virtual tstring  asString  ()                    const;
	virtual RDOValue value_cast(CREF(RDOValue) from) const;

	rbool           empty    () const;
	const CIterator begin    () const;
	const CIterator end      () const;
	CREF(Enums)     getValues() const;

private:
	Enums m_enum;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_enum.inl"

#endif //! _RDO_ENUM_H_
