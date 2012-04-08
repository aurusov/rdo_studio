/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.12.2009
  \brief     Тест массивов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
#define BOOST_TEST_MODULE RDORuntime_Array_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Array_Test)

typedef  rdo::vector<rsint>                                                Container;
typedef  std::pair<rdo::runtime::LPRDOArrayValue, rdo::runtime::RDOValue>  Array;

Array createArray(CREF(Container) data)
{
	rdo::runtime::LPRDOArrayType  pType  = rdo::Factory<rdo::runtime::RDOArrayType>::create(rdo::runtime::g_int);
	ASSERT(pType);
	rdo::runtime::LPRDOArrayValue pValue = rdo::Factory<rdo::runtime::RDOArrayValue>::create(pType);
	ASSERT(pValue);

	STL_FOR_ALL_CONST(data, it)
	{
		pValue->push_back(rdo::runtime::RDOValue(*it));
	}

	return std::make_pair(pValue, rdo::runtime::RDOValue(pType, pValue));
}

tstring getString(CREF(rdo::runtime::LPRDOArrayValue) pArray, CREF(rdo::runtime::LPRDOArrayIterator) pIt)
{
	if (!pIt->equal(pArray->end()))
	{
		return pIt->getValue().getAsString();
	}
	return _T("");
}

tstring getString(CREF(rdo::runtime::RDOValue) it, CREF(rdo::runtime::RDOValue) end)
{
	if (it != end)
	{
		return it.getAsString();
	}
	return _T("");
}

BOOST_AUTO_TEST_CASE(ArrayTestCreate)
{
	BOOST_CHECK(createArray(Container()(1)(2)(3)).second.getAsString() == _T("[1, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(ArrayTestInsert)
{
	Array array1 = createArray(Container()(1)(2)(3));
	Array array2 = createArray(Container()(4)(5)(6));

	array1.first->insert(array1.first->begin()->next(), array2.first->begin(), array2.first->end());

	BOOST_CHECK(array1.second.getAsString() == _T("[1, 4, 5, 6, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(ArrayTestErase)
{
	Array array = createArray(Container()(1)(2)(3));

	array.first->erase(array.first->begin()->next(), array.first->begin()->preInc(3));

	BOOST_CHECK(array.second.getAsString() == _T("[1]"));
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPrePlus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt = array.first->begin();
	while (!pIt->equal(array.first->end()))
	{
		result += getString(array.first, pIt->preInc(1));
	}
	BOOST_CHECK(result == _T("23"));
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPostPlus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt = array.first->begin();
	while (!pIt->equal(array.first->end()))
	{
		result += getString(array.first, pIt->postInc(1));
	}
	BOOST_CHECK(result == _T("123"));
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPreMinus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt = array.first->end();
	do
	{
		result += getString(array.first, pIt->preInc(-1));
	}
	while (!pIt->equal(array.first->begin()));

	BOOST_CHECK(result == _T("321"));
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPostMinus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt = array.first->end();
	do
	{
		result += getString(array.first, pIt->postInc(-1));
	}
	while (!pIt->equal(array.first->begin()));

	BOOST_CHECK(result == _T("32"));
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePrePlus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt  = array.first->begin();
	rdo::runtime::LPRDOArrayIterator pEnd = array.first->end  ();
	rdo::runtime::RDOValue it (pIt,  pIt );
	rdo::runtime::RDOValue end(pEnd, pEnd);
	BOOST_CHECK(it != end);

	while (it != end)
	{
		result += getString(++it, end);
	}
	BOOST_CHECK(result == _T("23"));
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePostPlus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt  = array.first->begin();
	rdo::runtime::LPRDOArrayIterator pEnd = array.first->end  ();
	rdo::runtime::RDOValue it (pIt,  pIt );
	rdo::runtime::RDOValue end(pEnd, pEnd);
	BOOST_CHECK(it != end);

	while (it != end)
	{
		result += getString(it++, end);
	}
	BOOST_CHECK(result == _T("123"));
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePreMinus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt    = array.first->end  ();
	rdo::runtime::LPRDOArrayIterator pBegin = array.first->begin();
	rdo::runtime::LPRDOArrayIterator pEnd   = array.first->end  ();
	rdo::runtime::RDOValue it   (pIt,    pIt   );
	rdo::runtime::RDOValue begin(pBegin, pBegin);
	rdo::runtime::RDOValue end  (pEnd,   pEnd  );
	BOOST_CHECK(it != begin);
	BOOST_CHECK(it == end  );

	do
	{
		result += getString(--it, end);
	}
	while (it != begin);
	BOOST_CHECK(result == _T("321"));
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePostMinus)
{
	Array array = createArray(Container()(1)(2)(3));

	tstring result;
	rdo::runtime::LPRDOArrayIterator pIt    = array.first->end  ();
	rdo::runtime::LPRDOArrayIterator pBegin = array.first->begin();
	rdo::runtime::LPRDOArrayIterator pEnd   = array.first->end  ();
	rdo::runtime::RDOValue it   (pIt,    pIt   );
	rdo::runtime::RDOValue begin(pBegin, pBegin);
	rdo::runtime::RDOValue end  (pEnd,   pEnd  );
	BOOST_CHECK(it != begin);
	BOOST_CHECK(it == end  );

	do
	{
		result += getString(it--, end);
	}
	while (it != begin);
	BOOST_CHECK(result == _T("32"));
}

BOOST_AUTO_TEST_CASE(ArrayTestSetItem)
{
	Array array = createArray(Container()(1)(2)(3));

	ruint ind  = 1;
	ruint item = 48;
	rdo::runtime::RDOValue index(ind);
	rdo::runtime::RDOValue value(item);
	array.first->setItem(index, value);

	BOOST_CHECK(array.second.getAsString() == _T("[1, 48, 3]"));

	ind         = 3;
	index       = ind;
	rbool found = false;

	try
	{
		array.first->setItem(index, value);
	}
	catch (CREF(rdo::runtime::RDORuntimeException) ex)
	{
		if (!ex.message().empty())
		{
			found = ex.message() == _T("Выход за пределы массива");
		}
	}

	if (!found)
	{
		BOOST_CHECK(false);
	}
}

BOOST_AUTO_TEST_CASE(ArrayTestGetItem)
{
	Array array = createArray(Container()(1)(48)(3));

	ruint ind = 1;
	rdo::runtime::RDOValue index(ind);
	rdo::runtime::RDOValue value(array.first->getItem(index));

	BOOST_CHECK(value.getAsString() == _T("48"));

	ind         = 3;
	index       = ind;
	rbool found = false;

	try
	{
		array.first->getItem(index);
	}
	catch (CREF(rdo::runtime::RDORuntimeException) ex)
	{
		if (!ex.message().empty())
		{
			found = ex.message() == _T("Выход за пределы массива");
		}
	}

	if (!found)
	{
		BOOST_CHECK(false);
	}
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Array_Test
