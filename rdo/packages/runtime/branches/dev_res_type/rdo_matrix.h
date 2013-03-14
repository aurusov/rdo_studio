/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_matrix.h
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.10.2010
  \brief     Матрица
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MATRIX_H_
#define _LIB_RUNTIME_MATRIX_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOMatrixType    );
PREDECLARE_POINTER(RDOMatrixIterator);

//! Элемент матрицы
OBJECT(RDOMatrixValue)
{
DECLARE_FACTORY(RDOMatrixValue)
public:
	typedef std::vector<RDOValue> Container;

	CREF(LPRDOMatrixType) type() const;

	void push_back(CREF(RDOValue) item);
	LPRDOMatrixIterator begin();
	LPRDOMatrixIterator end  ();
	void insert(CREF(LPRDOMatrixIterator) pWhere, CREF(LPRDOMatrixIterator) pFromFirst, CREF(LPRDOMatrixIterator) pFromLast);
	void  erase(CREF(LPRDOMatrixIterator) pFirst, CREF(LPRDOMatrixIterator) pLast);

	ruint   size       () const;
	tstring getAsString() const;

	CREF(RDOValue) getItem(CREF(RDOValue) index) const;
	void           setItem(CREF(RDOValue) index, CREF(RDOValue) item);

private:
	RDOMatrixValue(CREF(LPRDOMatrixType)  pType );
	RDOMatrixValue(CREF(LPRDOMatrixValue) pValue);
	virtual ~RDOMatrixValue();

	Container        m_container;
	LPRDOMatrixType  m_pMatrixType;
};

//! Итератор матрицы
class RDOMatrixIterator: public RDOType
{
DECLARE_FACTORY(RDOMatrixIterator)
public:
	typedef RDOMatrixValue::Container::iterator Iterator;

	Iterator             getIterator() const;
	CREF(RDOValue)       getValue   () const;
	LPRDOMatrixIterator  preInc     (rsint delta);
	LPRDOMatrixIterator  postInc    (rsint delta);
	LPRDOMatrixIterator  next       ();
	rbool                equal      (CREF(LPRDOMatrixIterator) pIterator) const;
	LPRDOMatrixIterator  clone      () const;

private:
	RDOMatrixIterator(CREF(LPRDOMatrixIterator) pIterator);
	RDOMatrixIterator(CREF(RDOMatrixIterator)   iterator );
	RDOMatrixIterator(CREF(Iterator)            iterator );
	virtual ~RDOMatrixIterator();

	Iterator  m_iterator;
};

//! Тип матрица
class RDOMatrixType: public RDOType
{
DECLARE_FACTORY(RDOMatrixType);
public:
	typedef  LPRDOType       LPItemType;
	typedef  RDOMatrixValue  value_type;

	CREF(LPItemType) getItemType() const;

private:
	RDOMatrixType(CREF(LPItemType) pItemType);
	virtual ~RDOMatrixType();

	LPItemType  m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_MATRIX_H_
