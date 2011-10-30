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

/*!
  \class     RDOArrayValue
  \brief     Элемент массива
*/
class RDOArrayValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOArrayValue(CREF(LPRDOArrayType) pType);
	RDOArrayValue(CREF(RDOArrayValue)  value);
	~RDOArrayValue();

	CREF(LPRDOArrayType) type() const;

	void insertItem(CREF(RDOValue) pArray);
	Container::iterator containerBegin();
	Container::iterator containerEnd();
	void insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                         );
	CREF(RDOValue) operator[] (CREF(RDOValue) ind) const;

	ruint arraySize() const;

	tstring getAsString() const;
	void setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item);

private:
	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};

/*!
  \class     RDOArrayIterator
  \brief     Итератор массива
*/
class RDOArrayIterator
{
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	RDOArrayIterator(CREF(RDOArrayIterator) iterator);
	RDOArrayIterator(CREF(Iterator)         iterator);

	Iterator getIterator() const;
	Iterator operator+  (rsint num);
	Iterator operator-  (rsint num);
	rbool    operator== (CREF(RDOArrayIterator) iterator) const;

	RDOValue getValue() const;

private:
	Iterator m_iterator;
};

/*!
  \class     RDOArrayType
  \brief     Тип массива
*/
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
