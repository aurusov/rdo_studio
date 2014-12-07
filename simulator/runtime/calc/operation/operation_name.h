#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <typename opr_type>
class OperatorName
{
public:
	static std::string name(const opr_type& pOperator);

private:
	struct OprItem
	{
		opr_type m_pOperator;
		std::string m_name;

		OprItem(const opr_type& pOperator, const std::string& name);

		bool operator== (const opr_type& pOperator) const;
	};
	typedef std::list<OprItem> NameList;

	static const NameList& getList();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/operation_name-inl.h"
