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
#include "utils/src/common/rdocommon.h"
#include "utils/src/smart_ptr/factory/factory.h"
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
	typedef std::string EnumItem;
	typedef rdo::vector<EnumItem> Enums;
	typedef Enums::const_iterator CIterator;
	typedef Enums::const_iterator const_iterator;

	static const std::size_t END = std::size_t(~0);

	void add(CREF(std::string) next);
	std::size_t findEnum(CREF(std::string) val) const;
	bool exist(CREF(std::string) val) const;

	bool empty() const;
	const CIterator begin() const;
	const CIterator end() const;
	CREF(Enums) getValues() const;

private:
	RDOEnumType();
	RDOEnumType(CREF(Enums) enums);

	Enums m_enum;
};
DECLARE_POINTER(RDOEnumType);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_enum.inl"

#endif // _LIB_RUNTIME_ENUM_H_
