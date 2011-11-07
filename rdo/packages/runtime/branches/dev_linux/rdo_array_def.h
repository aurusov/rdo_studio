/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_array_def.h
  \author    Чирков Михаил
  \date      14.12.2009
  \brief     Определение массива
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ARRAY_DEF_H_
#define _LIB_RUNTIME_ARRAY_DEF_H_

#ifndef _LIB_RUNTIME_VALUE_H_
#error include "simulator/runtime/rdo_value.h" first (вместо "rdo_array_def.h" надо инклюдить "rdo_array.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "simulator/runtime/rdo_array.h"
//

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOArrayType);

//! Элемент массива
OBJECT(RDOArrayValue)
{
DECLARE_FACTORY(RDOArrayValue)
public:
	typedef std::vector<RDOValue> Container;

	CREF(LPRDOArrayType) type() const;

	void insertItem(CREF(RDOValue) item);
	Container::iterator containerBegin();
	Container::iterator containerEnd  ();
	void insertItems(Container::iterator itr,    Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                            );
	CREF(RDOValue) operator[] (CREF(RDOValue) index) const;

	ruint   arraySize   () const;
	tstring getAsString () const;
	void    setArrayItem(CREF(RDOValue) index, CREF(RDOValue) item);

private:
	RDOArrayValue(CREF(LPRDOArrayType)  pType );
	RDOArrayValue(CREF(LPRDOArrayValue) pValue);
	virtual ~RDOArrayValue();
	
	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};

//! Итератор массива
OBJECT(RDOArrayIterator)
{
DECLARE_FACTORY(RDOArrayIterator)
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	Iterator getIterator() const;
	Iterator operator+  (rsint num);
	Iterator operator-  (rsint num);
	rbool    operator== (CREF(LPRDOArrayIterator) pIterator) const;

	RDOValue getValue() const;

private:
	RDOArrayIterator(CREF(LPRDOArrayIterator) pIterator);
	RDOArrayIterator(CREF(Iterator)           iterator );
	virtual ~RDOArrayIterator();
	
	Iterator m_iterator;
};

//! Тип массива
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	typedef LPRDOType LPItemType;

	LPItemType getItemType() const;

private:
	RDOArrayType(CREF(LPItemType) pItemType);

	LPItemType m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ARRAY_DEF_H_
