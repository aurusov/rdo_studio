/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/type/type_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.12.2012
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_TYPE_I_H_
#define _SIMULATOR_COMPILER_PARSER_TYPE_I_H_

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
PREDECLARE_POINTER(RDOType );
PREDECLARE_POINTER(RDOValue);

class IType
{
public:
	virtual std::string name() const = 0;
	virtual LPRDOType type_cast(
		const LPRDOType&        pFrom,
		const RDOParserSrcInfo& from_src_info,
		const RDOParserSrcInfo& to_src_info,
		const RDOParserSrcInfo& src_info
	) const = 0;
	virtual LPRDOValue  value_cast(
		const LPRDOValue&       pFrom,
		const RDOParserSrcInfo& to_src_info,
		const RDOParserSrcInfo& src_info
	) const = 0;
	//! calc_cast вызывается строго после type_cast, поэтому никаких RDOParserSrcInfo не передается
	virtual rdo::runtime::LPRDOCalc calc_cast(
		const rdo::runtime::LPRDOCalc& pCalc,
		const LPRDOType&               pType
	) const = 0;
	virtual rdo::runtime::RDOValue get_default() const = 0;
};
#define DECLARE_IType                                                                                                                                                                          \
	virtual std::string              name       () const;                                                                                                                                      \
	virtual LPRDOType                type_cast  (const LPRDOType&  pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const; \
	virtual LPRDOValue               value_cast (const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info,   const RDOParserSrcInfo& src_info) const;                                      \
	virtual rdo::runtime::LPRDOCalc  calc_cast  (const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const;                                                                          \
	virtual rdo::runtime::RDOValue   get_default() const;

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_TYPE_I_H_
