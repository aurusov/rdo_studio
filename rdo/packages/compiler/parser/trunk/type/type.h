/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/type/type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_TYPE_H_
#define _SIMULATOR_COMPILER_PARSER_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/type/type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOType
	: public virtual rdo::counter_reference
	, public IType
	, public IModelStructure
{
DECLARE_FACTORY(RDOType)
public:
	virtual CREF(rdo::runtime::LPRDOType)    type() const = 0;
	virtual rdo::runtime::RDOType::TypeID  typeID() const = 0;;

protected:
	RDOType();
	virtual ~RDOType();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_TYPE_H_
