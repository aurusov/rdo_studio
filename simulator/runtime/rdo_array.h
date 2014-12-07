#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_as_string_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOArrayType    );
PREDECLARE_POINTER(RDOArrayIterator);

PREDECLARE_POINTER(RDOArrayValue);
class RDOArrayValue: public IAsString
{
DECLARE_FACTORY(RDOArrayValue)
public:
	typedef std::vector<RDOValue> Container;

	const LPRDOArrayType& type() const;

	void push_back(const RDOValue& item);
	LPRDOArrayIterator begin();
	LPRDOArrayIterator end  ();
	void insert(const LPRDOArrayIterator& pWhere, const LPRDOArrayIterator& pFromFirst, const LPRDOArrayIterator& pFromLast);
	void  erase(const LPRDOArrayIterator& pFirst, const LPRDOArrayIterator& pLast);

	std::size_t size() const;

	const RDOValue& getItem(const RDOValue& index) const;
	void            setItem(const RDOValue& index, const RDOValue& item);

	LPRDOArrayValue clone() const;

	DECLARE_IAsString;

private:
	RDOArrayValue(const LPRDOArrayType&  pType );
	RDOArrayValue(const LPRDOArrayValue& pValue);
	virtual ~RDOArrayValue();

	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};

class RDOArrayIterator: public RDOType, public IAsString
{
DECLARE_FACTORY(RDOArrayIterator)
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	Iterator getIterator() const;
	const RDOValue& getValue() const;
	LPRDOArrayIterator preInc(int delta);
	LPRDOArrayIterator postInc(int delta);
	LPRDOArrayIterator next();
	bool equal(const LPRDOArrayIterator& pIterator) const;
	LPRDOArrayIterator clone() const;

	DECLARE_IAsString;

private:
	RDOArrayIterator(const LPRDOArrayIterator& pIterator);
	RDOArrayIterator(const RDOArrayIterator&   iterator );
	RDOArrayIterator(const Iterator&           iterator );
	virtual ~RDOArrayIterator();

	Iterator  m_iterator;
};

class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	typedef  LPRDOType      LPItemType;
	typedef  RDOArrayValue  value_type;

	const LPItemType& getItemType() const;

protected:
	RDOArrayType(const LPItemType& pItemType);
	virtual ~RDOArrayType();

private:
	LPItemType  m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE
