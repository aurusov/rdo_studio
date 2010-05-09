/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_H_
#define _RDOPARSER_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IType
// ----------------------------------------------------------------------------
class RDOValue;
class RDOType;
DECLARE_POINTER(RDOType);

class IType
{
public:
	virtual tstring   name() const = 0;
	virtual LPRDOType type_cast(
		CREF(LPRDOType)        from,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	virtual RDOValue  value_cast(
		CREF(RDOValue)         from,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	//! calc_cast вызывается строго после type_cast, поэтому никаких RDOParserSrcInfo не передается
	virtual PTR(rdoRuntime::RDOCalc) calc_cast(
		PTR(rdoRuntime::RDOCalc) pCalc,
		CREF(LPRDOType)          pType
	) const = 0;
};
#define DECLARE_IType                                                                                                                                                                   \
	virtual tstring                  name      () const;                                                                                                                                \
	virtual LPRDOType                type_cast (CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const; \
	virtual RDOValue                 value_cast(CREF(RDOValue)  from, CREF(RDOParserSrcInfo) to_src_info,   CREF(RDOParserSrcInfo) src_info)                                     const; \
	virtual PTR(rdoRuntime::RDOCalc) calc_cast (PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const;

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType: public IType, public IModelStructure
{
DECLARE_FACTORY(RDOType)
public:
	CREF(rdoRuntime::RDOType)        type() const { return *m_type;         }
	CPTR(rdoRuntime::RDOType) operator-> () const { return  m_type;         }

	rdoRuntime::RDOType::TypeID    typeID() const { return type().typeID(); }

	virtual PTR(rdoRuntime::RDOCalc) calc_cast(PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const
	{
		return pCalc;
	}

protected:
	RDOType(CPTR(rdoRuntime::RDOType) type)
		: m_type(type)
	{
		ASSERT(m_type);
	}
	virtual ~RDOType()
	{}

	CPTR(rdoRuntime::RDOType) m_type;
};
DECLARE_POINTER(RDOType);

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type)    \
class RDOType__##Type: public RDOType    \
{                                        \
private:                                 \
typedef RDOType parent_type;             \
public:                                  \
	RDOType__##Type():                   \
		RDOType(&rdoRuntime::g_##Type)   \
	{}                                   \
	~RDOType__##Type()                   \
	{}                                   \
	DECLARE_IType;                       \
	DECLARE_IModelStructure;             \
};                                       \
extern rdo::smart_ptr<RDOType__##Type> g_##Type;

DEFINE_ATOM_TYPE_PARSER(unknow       );
DEFINE_ATOM_TYPE_PARSER(identificator);
DEFINE_ATOM_TYPE_PARSER(int          );
DEFINE_ATOM_TYPE_PARSER(real         );
DEFINE_ATOM_TYPE_PARSER(bool         );
DEFINE_ATOM_TYPE_PARSER(string       );

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_H_
