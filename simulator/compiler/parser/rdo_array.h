#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayValue
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOArrayValue);
class RDOArrayValue
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayValue);
public:
	typedef std::vector<LPRDOValue> Container;

	void insertItem(const LPRDOValue& pValue);

	const LPRDOArrayType& getArrayType() const;
	 LPRDOArrayType& getArrayType();
	rdo::runtime::RDOValue getRArray() const;
	rdo::runtime::LPRDOArrayValue createRuntimeValue() const;
	std::string getAsString() const;
	const Container& getContainer() const;

private:
	RDOArrayValue(const LPRDOArrayType& pArrayType);
	virtual ~RDOArrayValue();

	Container      m_container;
	LPRDOArrayType m_pArrayType;
};

CLOSE_RDO_PARSER_NAMESPACE
