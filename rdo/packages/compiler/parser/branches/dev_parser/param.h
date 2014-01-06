/*!
  \copyright (c) RDO-Team, 2011
  \file      param.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

#ifndef _PARSER_PARAM_H_
#define _PARSER_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParam
// --------------------------------------------------------------------------------
class RDOParam
	: public virtual rdo::counter_reference
	, public RDOParserSrcInfo
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOParam)
public:
	static const std::string CONTEXT_PARAM_PARAM_ID;

	CREF(tstring)    name       () const { return src_info().src_text(); }
	LPTypeInfo       getTypeInfo() const { return m_pType;               }
	CREF(LPRDOValue) getDefault () const { return m_pDefault;            }

protected:
	RDOParam(CREF(tstring)          name,    CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	RDOParam(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	virtual ~RDOParam();

private:
	LPTypeInfo m_pType;
	LPRDOValue m_pDefault;

	void checkDefault();

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDOParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _PARSER_PARAM_H_
