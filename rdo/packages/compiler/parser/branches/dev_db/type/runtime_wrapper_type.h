/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/type/runtime_wrapper_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.12.2012
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_
#define _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/type.h"
#include "simulator/compiler/parser/headers/db/interface_db.h"
#include "simulator/compiler/parser/type/type_db_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RuntimeWrapperType: public RDOType
{
DECLARE_FACTORY(RuntimeWrapperType)
public:
	virtual CREF(rdo::runtime::LPRDOType)   type() const;
	virtual rdo::runtime::RDOType::TypeID typeID() const;
	virtual void serializeInDB(REF(IDB) db) const;

	virtual rdo::runtime::LPRDOCalc calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;

protected:
	RuntimeWrapperType();
	RuntimeWrapperType(CREF(rdo::runtime::LPRDOType) pType);
	virtual ~RuntimeWrapperType();

	rdo::runtime::LPRDOType m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_
