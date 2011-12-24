/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      28.09.2009
  \brief     Тест логик
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <algorithm>
#define BOOST_TEST_MODULE RDORuntime_Logic_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "utils/rdodebug.h"
// --------------------------------------------------------------------------------

class MyOperation
{
public:
	MyOperation(ruint dummy)
		: m_dummy(dummy)
	{}

	rbool operator< (CREF(MyOperation) opr) const
	{
		return m_dummy < opr.m_dummy;
	}

	ruint getData()
	{
		return m_dummy;
	}

private:
	ruint m_dummy;
};

template <class T, class C = std::vector<T> >
class Container
{
public:
	typedef T                             Item;
	typedef C                             List;
	typedef typename List::iterator       Iterator;
	typedef typename List::const_iterator CIterator;

	Iterator  begin()       { return m_list.begin(); }
	Iterator  end  ()       { return m_list.end  (); }
	CIterator begin() const { return m_list.begin(); }
	CIterator end  () const { return m_list.end  (); }
	rbool     empty() const { return m_list.empty(); }

	REF(Container) operator() (CREF(Item) item)
	{
		m_list.push_back(item);
		return *this;
	}

private:
	List m_list;
};

class OrderFIFO
{
public:
	template <class Container>
	static void sort(REF(Container))
	{}
};

class OrderLIFO
{
public:
	template <class Container>
	static void sort(REF(Container) container)
	{
		std::reverse(container.begin(), container.end());
	}
};

class OrderPrior
{
public:
	template <class Container>
	static void sort(REF(Container) container)
	{
		std::sort(container.begin(), container.end());
	}
};

typedef std::vector<int> data_vector;

template <class Order>
class Logic
{
public:
	Logic(const data_vector& data)
	{
		data_vector::const_iterator endIt = data.end();
		for(data_vector::const_iterator it = data.begin(); it != endIt; ++it) 
		{
			m_container(*it);
		}
	}
	rbool checkOperation(const data_vector& data)
	{
		Order::sort(m_container);

		data_vector check_data;

		DataContainer::CIterator endIt = m_container.end();
		for(DataContainer::Iterator it = m_container.begin() ;it != endIt; ++it)
		{
			check_data.push_back((*it).getData());
		}

		return check_data == data;
	}

private:
	typedef MyOperation              Item;
	typedef Container<Item>          DataContainer;

	DataContainer  m_container;
};

data_vector createData()
{
	data_vector _vector;
	_vector.push_back(1);
	_vector.push_back(2);
	_vector.push_back(4);
	_vector.push_back(3);
	return _vector;
}

BOOST_AUTO_TEST_SUITE(RDORuntime_Logic_Test)

BOOST_AUTO_TEST_CASE(RDORuntime_Logic_FIFO_Test)
{
	Logic<OrderFIFO> logic(createData()); // 1 2 4 3

	data_vector checkVector;
	checkVector.push_back(1);
	checkVector.push_back(2);
	checkVector.push_back(4);
	checkVector.push_back(3);

	BOOST_CHECK(logic.checkOperation(checkVector));
}

BOOST_AUTO_TEST_CASE(RDORuntime_Logic_LIFO_Test)
{
	Logic<OrderLIFO> logic(createData()); // 3 4 2 1

	data_vector checkVector;
	checkVector.push_back(3);
	checkVector.push_back(4);
	checkVector.push_back(2);
	checkVector.push_back(1);

	BOOST_CHECK(logic.checkOperation(checkVector));
}

BOOST_AUTO_TEST_CASE(RDORuntime_Logic_Prior_Test)
{
	Logic<OrderPrior> logic(createData()); // 1 2 3 4

	data_vector checkVector;
	checkVector.push_back(1);
	checkVector.push_back(2);
	checkVector.push_back(3);
	checkVector.push_back(4);

	BOOST_CHECK(logic.checkOperation(checkVector));
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Logic_Test
