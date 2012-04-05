/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_TYPE_H_
#define _CONVERTOR_RDOCONVERTER_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/calc/calc_base.h"
#include "converter/smr2rdox/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOValue);
PREDECLARE_POINTER(RDOType );

class IType
{
public:
	virtual tstring    name() const = 0;
	virtual LPRDOType  type_cast(
		CREF(LPRDOType)        pFrom,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	virtual LPRDOValue value_cast(
		CREF(LPRDOValue)       pFrom,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	//! calc_cast вызывается строго после type_cast, поэтому никаких RDOParserSrcInfo не передается
	virtual rdoRuntime::LPRDOCalc calc_cast(
		CREF(rdoRuntime::LPRDOCalc) pCalc,
		CREF(LPRDOType)             pType
	) const = 0;
	virtual LPRDOValue get_default() const = 0;
};
#define DECLARE_ITypeConverter                                                                                                                                                           \
	virtual tstring                name       () const;                                                                                                                                  \
	virtual LPRDOType              type_cast  (CREF(LPRDOType)  pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const; \
	virtual LPRDOValue             value_cast (CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info,   CREF(RDOParserSrcInfo) src_info)                                     const; \
	virtual rdoRuntime::LPRDOCalc  calc_cast  (CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;                                                                          \
	virtual LPRDOValue             get_default() const;

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------
OBJECT(RDOType)
	IS  IMPLEMENTATION_OF(IType          )
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
	}
	virtual ~RDOType()
	{}

	rdoRuntime::LPRDOType m_pType;
};

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_CONVERTER
// --------------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_CONVERTER(Type) \
class RDOType__##Type: public RDOType    \
{                                        \
private:                                 \
typedef RDOType parent_type;             \
public:                                  \
	RDOType__##Type():                   \
		RDOType(rdoRuntime::g_##Type)    \
	{}                                   \
	~RDOType__##Type()                   \
	{}                                   \
	DECLARE_ITypeConverter;              \
	DECLARE_IModelStructure;             \
};                                       \
DECLARE_POINTER(RDOType__##Type);

DEFINE_ATOM_TYPE_CONVERTER(unknow       );
DEFINE_ATOM_TYPE_CONVERTER(identificator);
DEFINE_ATOM_TYPE_CONVERTER(int          );
DEFINE_ATOM_TYPE_CONVERTER(real         );
DEFINE_ATOM_TYPE_CONVERTER(bool         );
DEFINE_ATOM_TYPE_CONVERTER(string       );

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_TYPE_H_
