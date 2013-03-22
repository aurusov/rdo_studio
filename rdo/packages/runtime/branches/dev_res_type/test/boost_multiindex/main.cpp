#define BOOST_TEST_MODULE multiindex_test

#include <iostream>
#include <stdio.h>

#include <boost/test/included/unit_test.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>

#include <boost/shared_ptr.hpp>

using namespace boost::multi_index;
using boost::multi_index_container;
using boost::multi_index::identity;
using boost::multi_index::member;

struct employee
{
  int  id;
  boost::shared_ptr<int> param;

  employee(int id, boost::shared_ptr<int> i):id(id),param(i){}

  bool operator<(const employee& e)const{return id<e.id;}

employee& operator=(const employee& right) {
  if (this == &right) {
    return *this;
  }
  param = right.param;
  return *this;
}

};

typedef multi_index_container<
	employee,
	indexed_by<
		random_access<>,
		ordered_unique<member<employee,int,&employee::id>>
	>
	> employee_set;


class {
public:
	template<typename T>
	operator boost::shared_ptr<T>() { return boost::shared_ptr<T>(); }
} nullPtr;

BOOST_AUTO_TEST_SUITE(multiindex_test)

BOOST_AUTO_TEST_CASE(always_true)
{
	employee_set es;

	boost::shared_ptr<int> i1(new int(5));
	boost::shared_ptr<int> i2(new int(6));

	es.push_back(employee(3, i1));
	es.push_back(employee(1, i2));

	for (int i=0; i<es.size(); i++)
	{
		std::cout << es[i].id << "	" << es[i].param << std::endl;
	}

	std::cout << std::endl;

	i2 = nullPtr;
	es.push_back(employee(2, i2));
	
	for (int i=0; i<es.size(); i++)
	{
		std::cout << es[i].id << "	" << es[i].param << std::endl;
	}

	BOOST_CHECK(es.size()==3);

}



BOOST_AUTO_TEST_SUITE_END() // multiindex_test