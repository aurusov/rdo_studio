/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/type/type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_H_
#define _RDOPARSER_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_model_i.h"
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

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------
OBJECT_VIRTUAL(RDOType)
	IS  IMPLEMENTATION_OF(IType          )
	AND IMPLEMENTATION_OF(IModelStructure)
{
DECLARE_FACTORY(RDOType)
public:
	CREF(rdo::runtime::LPRDOType)    type() const;
	rdo::runtime::RDOType::TypeID  typeID() const;

	virtual rdo::runtime::LPRDOCalc calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;

protected:
	RDOType();
	RDOType(CREF(rdo::runtime::LPRDOType) pType);
	virtual ~RDOType();

	rdo::runtime::LPRDOType m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_H_
