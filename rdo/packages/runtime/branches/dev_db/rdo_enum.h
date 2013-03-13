/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_enum.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Перечисления
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ENUM_H_
#define _LIB_RUNTIME_ENUM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/smart_ptr/factory.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOEnumType
  \brief     Тип перечисления
*/
class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	typedef tstring                EnumItem;
	typedef rdo::vector<EnumItem>  Enums;
	typedef Enums::const_iterator  CIterator;
	typedef Enums::const_iterator  const_iterator;

	static const ruint END = ruint(~0);

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

#include "simulator/runtime/rdo_enum.inl"

#endif // _LIB_RUNTIME_ENUM_H_
