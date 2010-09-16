/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array_def.h
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */
#ifndef _RDO_ARRAY_DEF_H_
#define _RDO_ARRAY_DEF_H_

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (вместо "rdo_array_def.h" надо инклюдить "rdo_array.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "rdo_array.h"
//

// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayType;

class RDOArrayValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOArrayValue(CREF(RDOArrayType)  type );
	RDOArrayValue(CREF(RDOArrayValue) value);
	~RDOArrayValue();

	CREF(RDOArrayType) type() const;

	void insertItem(CREF(RDOValue) pArray);
	Container::iterator containerBegin();
	Container::iterator containerEnd();
	void insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                         );
	CREF(RDOValue) operator[] (CREF(RDOValue) ind);

	tstring getAsString() const;

private:
	Container          m_container;
	CPTR(RDOArrayType) m_arrayType;
};

// ----------------------------------------------------------------------------
// ---------- RDOArrayIterator
// ----------------------------------------------------------------------------
class RDOArrayIterator
{
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	RDOArrayIterator(CREF(RDOArrayIterator) iterator);
	RDOArrayIterator(CREF(Iterator)         iterator);

	Iterator getIterator() const;
	Iterator operator+  (rsint num);

private:
	Iterator m_iterator;
};

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDORuntimeObject
{
public:
	typedef CREF(RDOType) ArrayType;

	RDOArrayType(PTR(RDORuntimeParent) parent, ArrayType pArrayType);

	ArrayType getArrayType() const;

private:
	ArrayType  m_pArrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ARRAY_DEF_H_
