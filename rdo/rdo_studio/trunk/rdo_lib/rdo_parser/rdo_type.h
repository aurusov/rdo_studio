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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOTypeRange;

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType
{
public:
	RDOType(CREF(rdoRuntime::RDOType) type)
		: m_type (&type)
	{}
	CREF(rdoRuntime::RDOType)        type() const { return *m_type; }
	CPTR(rdoRuntime::RDOType) operator-> () const { return  m_type; }

	CPTR(RDOType) type_cast_throw(
		CREF(RDOType)          from,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const;

	virtual tstring              name      ()                                const = 0;
	virtual CPTR(RDOType)        type_cast (CREF(RDOType) toType)            const = 0;
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const = 0;

	static CREF(RDOType) getTypeByID(rdoRuntime::RDOType::TypeID typeID);

protected:
	CPTR(rdoRuntime::RDOType) m_type;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Class)                                                                    \
class RDOType__##Class: public RDOType                                                                    \
{                                                                                                         \
public:                                                                                                   \
	RDOType__##Class(): RDOType(rdoRuntime::g_##Class) {}                                                 \
	virtual tstring              name      ()                                const { return ""#Class""; } \
	virtual CPTR(RDOType)        type_cast (CREF(RDOType) toType)            const;                       \
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const;                       \
};                                                                                                        \
extern RDOType__##Class g_##Class;

DEFINE_ATOM_TYPE_PARSER(unknow       );
DEFINE_ATOM_TYPE_PARSER(identificator);
DEFINE_ATOM_TYPE_PARSER(int          );
DEFINE_ATOM_TYPE_PARSER(real         );
DEFINE_ATOM_TYPE_PARSER(bool         );
DEFINE_ATOM_TYPE_PARSER(string       );

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_H_
