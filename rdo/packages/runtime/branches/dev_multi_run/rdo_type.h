/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Базовый тип данных
  \indent    4T
*/

#ifndef _LIB_RUNTIME_TYPE_H_
#define _LIB_RUNTIME_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOValue;

/*!
  \class     RDOType
  \brief     Базовый тип данных РДО
*/
OBJECT(RDOType)
{
public:
	/*!
	  \enum      TypeID
	  \brief     Идентификатор типа данных
	*/
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
		t_pointer
	};

	RDOType(TypeID typeID);
	virtual ~RDOType();

	TypeID  typeID() const;

private:
	TypeID  m_typeID;
};

/// @todo внимание макрос!
/*!
  \def       DEFINE_ATOM_TYPE
  \brief     Макрос для определения простых (атомарных) типов данных
*/
#define DEFINE_ATOM_TYPE(Type)        \
class RDOType__##Type: public RDOType \
{                                     \
public:                               \
	RDOType__##Type()                 \
		: RDOType(t_##Type)           \
	{}                                \
};                                    \
extern rdo::intrusive_ptr<RDOType__##Type> g_##Type;

/*!
  \class     RDOType__unknow
  \brief     Атомарный неизвестный тип данных
*/
DEFINE_ATOM_TYPE(unknow);

/*!
  \class     RDOType__void
  \brief     Атомарный пусто тип данных
*/
DEFINE_ATOM_TYPE(void);

/*!
  \class     RDOType__identificator
  \brief     Атомарный тип данных идентификатор
*/
DEFINE_ATOM_TYPE(identificator);

/*!
  \class     RDOType__int
  \brief     Атомарный целочисленный тип данных
*/
DEFINE_ATOM_TYPE(int);

/*!
  \class     RDOType__real
  \brief     Атомарный вещественный тип данных
*/
DEFINE_ATOM_TYPE(real);

/*!
  \class     RDOType__bool
  \brief     Атомарный логический тип данных
*/
DEFINE_ATOM_TYPE(bool);

/*!
  \class     RDOType__string
  \brief     Атомарный строковый тип данных
*/
DEFINE_ATOM_TYPE(string);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_type.inl"

#endif // _LIB_RUNTIME_TYPE_H_
