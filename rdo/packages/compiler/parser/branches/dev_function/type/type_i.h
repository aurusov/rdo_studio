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
#include "utils/smart_ptr/factory.h"
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
	virtual tstring   name() const = 0;
	virtual LPRDOType type_cast(
		CREF(LPRDOType)        pFrom,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	virtual LPRDOValue  value_cast(
		CREF(LPRDOValue)       pFrom,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	//! calc_cast вызывается строго после type_cast, поэтому никаких RDOParserSrcInfo не передается
	virtual rdo::runtime::LPRDOCalc calc_cast(
		CREF(rdo::runtime::LPRDOCalc) pCalc,
		CREF(LPRDOType)               pType
	) const = 0;
	virtual rdo::runtime::RDOValue get_default() const = 0;
};
#define DECLARE_IType                                                                                                                                                                      \
	virtual tstring                  name       () const;                                                                                                                                  \
	virtual LPRDOType                type_cast  (CREF(LPRDOType)  pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const; \
	virtual LPRDOValue               value_cast (CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info,   CREF(RDOParserSrcInfo) src_info)                                     const; \
	virtual rdo::runtime::LPRDOCalc  calc_cast  (CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;                                                                        \
	virtual rdo::runtime::RDOValue   get_default() const;

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_TYPE_I_H_
