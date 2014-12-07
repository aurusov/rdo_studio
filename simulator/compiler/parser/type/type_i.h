#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOType);
PREDECLARE_POINTER(RDOValue);
PREDECLARE_POINTER(IType);
class IType: public virtual rdo::counter_reference
{
public:
	virtual std::string name() const = 0;
	virtual LPIType type_cast(
		const LPIType& pFrom,
		const RDOParserSrcInfo& from_src_info,
		const RDOParserSrcInfo& to_src_info,
		const RDOParserSrcInfo& src_info
	) const = 0;
	virtual LPRDOValue value_cast(
		const LPRDOValue& pFrom,
		const RDOParserSrcInfo& to_src_info,
		const RDOParserSrcInfo& src_info
	) const = 0;
	// calc_cast вызывается строго после type_cast, поэтому никаких RDOParserSrcInfo не передается
	virtual rdo::runtime::LPRDOCalc calc_cast(
		const rdo::runtime::LPRDOCalc& pCalc,
		const LPIType& pType
	) const = 0;
	virtual rdo::runtime::RDOValue get_default() const = 0;

	virtual ~IType()
	{}
};
#define DECLARE_IType                                                                                                                                                    \
	virtual std::string name() const;                                                                                                                                    \
	virtual LPIType type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const; \
	virtual LPRDOValue value_cast (const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info,   const RDOParserSrcInfo& src_info) const;                              \
	virtual rdo::runtime::LPRDOCalc calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const;                                                         \
	virtual rdo::runtime::RDOValue get_default() const;

CLOSE_RDO_PARSER_NAMESPACE
