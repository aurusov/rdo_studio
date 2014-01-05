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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParam
// --------------------------------------------------------------------------------
class RDOParam
	: public virtual rdo::counter_reference
	, public RDOParserSrcInfo
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
};
DECLARE_POINTER(RDOParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _PARSER_PARAM_H_
