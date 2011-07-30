/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_enum.h
 * @authors   Урусов Андрей
 * @date      
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_ENUM_H_
#define _RDO_ENUM_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOEnumType
// ********************************************************************************
class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	typedef tstring                EnumItem;
	typedef rdo::vector<EnumItem>  Enums;
	typedef Enums::const_iterator  CIterator;
	typedef Enums::const_iterator  const_iterator;

	enum { END = ~0 };

	void  add     (CREF(tstring) next);
	ruint findEnum(CREF(tstring) val) const;
	rbool exist   (CREF(tstring) val) const;

	rbool           empty    () const;
	const CIterator begin    () const;
	const CIterator end      () const;
	CREF(Enums)     getValues() const;

private:
	RDOEnumType();
	RDOEnumType(CREF(Enums) enums);

	Enums m_enum;
};
DECLARE_POINTER(RDOEnumType);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_enum.inl"

#endif // _RDO_ENUM_H_
