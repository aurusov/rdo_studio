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
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/type/type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

OBJECT_VIRTUAL(RDOType)
	IS  IMPLEMENTATION_OF(IType          )
	AND IMPLEMENTATION_OF(IModelStructure)
{
DECLARE_FACTORY(RDOType)
public:
	virtual CREF(rdo::runtime::LPRDOType)    type() const;
	virtual rdo::runtime::RDOType::TypeID  typeID() const;

	virtual rdo::runtime::LPRDOCalc calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;

protected:
	RDOType();
	RDOType(CREF(rdo::runtime::LPRDOType) pType);
	virtual ~RDOType();

	rdo::runtime::LPRDOType m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_H_
