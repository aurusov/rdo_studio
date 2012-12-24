/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator\compiler\parser\type\type_i_db.h
  \author    Клеванец Игорь (cerevra@gmail.com)
  \date      24.12.2012
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_I_TYPE_DB_H_
#define _SIMULATOR_COMPILER_PARSER_I_TYPE_DB_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/headers/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class ISerializeTypeInDB
{
public:
	virtual int selfInsertToDB(REF(InterfaceDB) db) const = 0;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_I_TYPE_DB_H_
