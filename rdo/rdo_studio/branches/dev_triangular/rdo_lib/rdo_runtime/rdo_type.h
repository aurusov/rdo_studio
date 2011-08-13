/******************************************************************************//**
 * @copyright (c) RDO-Team, 2008
 * @file      rdo_type.h
 * @authors   Урусов Андрей
 * @date      22.06.2008
 * @brief     Базовый тип данных
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_TYPE_H_
#define _LIB_RUNTIME_TYPE_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

class RDOValue;

/******************************************************************************//**
 * @class     RDOType
 * @brief     Базовый тип данных РДО
 *********************************************************************************/
OBJECT(RDOType)
{
public:
	/******************************************************************************//**
	 * @enum      TypeID
	 * @brief     Идентификатор типа данных
	 *********************************************************************************/
	enum TypeID
	{
		t_unknow = 0,
		t_void,
		t_identificator,
		t_int,
		t_real,
		t_bool,
		t_string,
		t_enum,
		t_array,
		t_arrayIterator,
		t_matrix,
		t_matrixIterator,
		t_fuzzy
	};

	RDOType(TypeID typeID);

	TypeID  typeID() const;

private:
	TypeID  m_typeID;
};

/// @todo внимание макрос!
/******************************************************************************//**
 * @def       DEFINE_ATOM_TYPE
 * @brief     Макрос для определения простых (атомарных) типов данных
 *********************************************************************************/
#define DEFINE_ATOM_TYPE(Type)        \
class RDOType__##Type: public RDOType \
{                                     \
public:                               \
	RDOType__##Type()                 \
		: RDOType(t_##Type)           \
	{}                                \
};                                    \
extern rdo::intrusive_ptr<RDOType__##Type> g_##Type;

DEFINE_ATOM_TYPE(unknow        );
DEFINE_ATOM_TYPE(void          );
DEFINE_ATOM_TYPE(identificator );
DEFINE_ATOM_TYPE(int           );
DEFINE_ATOM_TYPE(real          );
DEFINE_ATOM_TYPE(bool          );
DEFINE_ATOM_TYPE(string        );
DEFINE_ATOM_TYPE(arrayIterator );
DEFINE_ATOM_TYPE(matrixIterator);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_type.inl"

#endif // _LIB_RUNTIME_TYPE_H_
