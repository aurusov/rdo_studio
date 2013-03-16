/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator\runtime\type\type_i_db.h
  \author    Клеванец Игорь (cerevra@gmail.com)
  \date      24.12.2012
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_RUNTIME_I_TYPE_DB_H_
#define _SIMULATOR_RUNTIME_I_TYPE_DB_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/headers/db/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class ISerializeTypeInDB
{
public:
	virtual void serializeInDB(REF(IDB) db) const = 0;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _SIMULATOR_RUNTIME_I_TYPE_DB_H_
