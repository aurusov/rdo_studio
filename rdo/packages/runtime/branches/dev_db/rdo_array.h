/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_array.h
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2009
  \brief     Массив
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ARRAY_H_
#define _LIB_RUNTIME_ARRAY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOArrayType    );
PREDECLARE_POINTER(RDOArrayIterator);

//! Элемент массива
OBJECT(RDOArrayValue)
{
DECLARE_FACTORY(RDOArrayValue)
public:
	typedef std::vector<RDOValue> Container;

	CREF(LPRDOArrayType) type() const;

	void push_back(CREF(RDOValue) item);
	LPRDOArrayIterator begin();
	LPRDOArrayIterator end  ();
	void insert(CREF(LPRDOArrayIterator) pWhere, CREF(LPRDOArrayIterator) pFromFirst, CREF(LPRDOArrayIterator) pFromLast);
	void  erase(CREF(LPRDOArrayIterator) pFirst, CREF(LPRDOArrayIterator) pLast);

	ruint   size       () const;
	tstring getAsString() const;

	CREF(RDOValue) getItem(CREF(RDOValue) index) const;
	void           setItem(CREF(RDOValue) index, CREF(RDOValue) item);

	LPRDOArrayValue clone() const;

#ifdef INTERSECTION_DB
	void serializeInDB(REF(IDB) db) const;
#endif
#ifdef SERIALIZE_IN_DB_MAINSTREAM
	void RDOArrayValue::updateInDB(ruint index, ruint traceID, REF(IDB) db, CREF(RDOValue) value);
#endif

private:
	RDOArrayValue(CREF(LPRDOArrayType)  pType );
	RDOArrayValue(CREF(LPRDOArrayValue) pValue);
	virtual ~RDOArrayValue();

	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};

//! Итератор массива
class RDOArrayIterator: public RDOType
{
DECLARE_FACTORY(RDOArrayIterator)
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	Iterator            getIterator() const;
	CREF(RDOValue)      getValue   () const;
	LPRDOArrayIterator  preInc     (rsint delta);
	LPRDOArrayIterator  postInc    (rsint delta);
	LPRDOArrayIterator  next       ();
	rbool               equal      (CREF(LPRDOArrayIterator) pIterator) const;
	LPRDOArrayIterator  clone      () const;

private:
	RDOArrayIterator(CREF(LPRDOArrayIterator) pIterator);
	RDOArrayIterator(CREF(RDOArrayIterator)   iterator );
	RDOArrayIterator(CREF(Iterator)           iterator );
	virtual ~RDOArrayIterator();

	Iterator  m_iterator;
};

//! Тип массив
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	typedef  LPRDOType      LPItemType;
	typedef  RDOArrayValue  value_type;

	CREF(LPItemType) getItemType() const;

private:
	RDOArrayType(CREF(LPItemType) pItemType);
	virtual ~RDOArrayType();

	LPItemType  m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ARRAY_H_
