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
	virtual rdoRuntime::LPRDOCalc calc_cast(
		CREF(rdoRuntime::LPRDOCalc) pCalc,
		CREF(LPRDOType)             pType
	) const = 0;
	virtual rdoRuntime::RDOValue get_default() const = 0;
};
#define DECLARE_IType                                                                                                                                                                    \
	virtual tstring                name       () const;                                                                                                                                  \
	virtual LPRDOType              type_cast  (CREF(LPRDOType)  pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const; \
	virtual LPRDOValue             value_cast (CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info,   CREF(RDOParserSrcInfo) src_info)                                     const; \
	virtual rdoRuntime::LPRDOCalc  calc_cast  (CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;                                                                          \
	virtual rdoRuntime::RDOValue   get_default() const;

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------
class RDOType: virtual public rdo::counter_reference
	AND IMPLEMENTATION_OF(IType          )
	AND IMPLEMENTATION_OF(IModelStructure)
{
DECLARE_FACTORY(RDOType)
public:
	CREF(rdoRuntime::LPRDOType)        type() const { return m_pType;           }
	CREF(rdoRuntime::LPRDOType) operator-> () const { return m_pType;           }

	rdoRuntime::RDOType::TypeID      typeID() const { return m_pType->typeID(); }

	virtual rdoRuntime::LPRDOCalc calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
	{
		UNUSED(pType);
		return pCalc;
	}

protected:
	RDOType(CREF(rdoRuntime::LPRDOType) pType)
		: m_pType(pType)
	{
		ASSERT(m_pType);
		if (typeID() == rdoRuntime::RDOType::t_unknow)
		{
			int i = 1;
			UNUSED(i);
		}
	}
	inline RDOType()
	{}
	virtual ~RDOType()
	{}

	rdoRuntime::LPRDOType m_pType;
};
DECLARE_POINTER(RDOType);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_H_
