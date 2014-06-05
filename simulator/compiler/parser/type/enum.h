/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ENUM_H_
#define _RDOPARSER_TYPE_ENUM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOEnumType);

class RDOEnumType
	: public rdo::runtime::RDOEnumType
	, public IType
	, public IModelStructure
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(const LPRDOValue& pNext);

	bool operator== (const RDOEnumType& other) const
	{
		return getValues() == other.getValues();
	}
	bool operator!= (const RDOEnumType& other) const
	{
		return !operator==(other);
	}

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOEnumType();
	virtual ~RDOEnumType();

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ENUM_H_
