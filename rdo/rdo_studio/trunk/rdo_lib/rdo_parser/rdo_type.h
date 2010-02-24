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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType: public IModelStructure
{
public:
	RDOType(CPTR(rdoRuntime::RDOType) type)
		: m_type(type)
	{
		ASSERT(m_type);
	}
	CREF(rdoRuntime::RDOType)        type() const { return *m_type; }
	CPTR(rdoRuntime::RDOType) operator-> () const { return  m_type; }

	CPTR(RDOType) type_cast_throw(
		CREF(RDOType)          from,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const;

	virtual tstring              name      ()                                const = 0;
	virtual CPTR(RDOType)        type_cast (CREF(RDOType)              from) const = 0;
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const = 0;

	static CREF(RDOType) getTypeByID(rdoRuntime::RDOType::TypeID typeID);

protected:
	CPTR(rdoRuntime::RDOType) m_type;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type, TypeName)                                                         \
class RDOType__##Type: public RDOType                                                                   \
{                                                                                                       \
public:                                                                                                 \
	RDOType__##Type():                                                                                  \
		RDOType(&rdoRuntime::g_##Type)                                                                  \
	{}                                                                                                  \
	virtual tstring              name      ()                                const { return TypeName; } \
	virtual CPTR(RDOType)        type_cast (CREF(RDOType)              from) const;                     \
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const;                     \
	DECLARE_IModelStructure;                                                                            \
};                                                                                                      \
extern RDOType__##Type g_##Type;

DEFINE_ATOM_TYPE_PARSER(unknow,        _T("unknow")       );
DEFINE_ATOM_TYPE_PARSER(identificator, _T("identificator"));
DEFINE_ATOM_TYPE_PARSER(int,           _T("integer")      );
DEFINE_ATOM_TYPE_PARSER(real,          _T("real")         );
DEFINE_ATOM_TYPE_PARSER(bool,          _T("bool")         );
DEFINE_ATOM_TYPE_PARSER(string,        _T("string")       );

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_H_
