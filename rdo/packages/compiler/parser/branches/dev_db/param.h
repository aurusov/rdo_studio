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
#include "simulator/compiler/parser/context/context_switch_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParam
// --------------------------------------------------------------------------------
CLASS(RDOParam):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextSwitch  )
{
DECLARE_FACTORY(RDOParam)
public:
	CREF(tstring)    name       () const;
	LPTypeInfo       getTypeInfo() const;
	CREF(LPRDOValue) getDefault () const;

protected:
	RDOParam(CREF(tstring)          name,    CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	RDOParam(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	virtual ~RDOParam();

private:
	LPTypeInfo m_pType;
	LPRDOValue m_pDefault;

	void checkDefault();

	DECLARE_IContextSwitch;
};
DECLARE_POINTER(RDOParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _PARSER_PARAM_H_
