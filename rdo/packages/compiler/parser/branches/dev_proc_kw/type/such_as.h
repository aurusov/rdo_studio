/*!
  \copyright (c) RDO-Team, 2011
  \file      such_as.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_SUCH_AS_H_
#define _RDOPARSER_TYPE_SUCH_AS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/param.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
CLASS(RDOTypeParamSuchAs): INSTANCE_OF(TypeInfo)
{
DECLARE_FACTORY(RDOTypeParamSuchAs);
public:
	CREF(LPRDOParam) getParam() const;

private:
	RDOTypeParamSuchAs(CREF(LPRDOParam) pParam);
	virtual ~RDOTypeParamSuchAs();

	LPRDOParam m_pParam;
};
DECLARE_POINTER(RDOTypeParamSuchAs);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_SUCH_AS_H_
