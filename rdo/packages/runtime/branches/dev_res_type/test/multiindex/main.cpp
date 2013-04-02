#define BOOST_TEST_MODULE multiindex_test

#include <iostream>
#include <stdio.h>

#include <boost/test/included/unit_test.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <boost/shared_ptr.hpp>

using namespace boost::multi_index;
using boost::multi_index_container;
using boost::multi_index::member;

class {
public:
	template<typename T>
	operator boost::shared_ptr<T>() { return boost::shared_ptr<T>(); }
} nullPtr;

struct employee
{
	int         id;
	std::string name;
	int         age;

	employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}
};

struct t_employee
{
	int                         type;
	boost::shared_ptr<employee> e;

	t_employee(int type_,int id_,std::string name_,int age_):type(type_),e(new employee(id_,name_,age_)){}

	t_employee():type(0),e(nullPtr){}

	t_employee(int type_,employee employee_):type(type_),e(new employee(employee_.id,employee_.name,employee_.age)){}

	boost::shared_ptr<employee> operator->() const
	{
		return e;
	}

	employee& operator *() const
	{
		return *e;
	}
};

typedef multi_index_container<
	t_employee,
		indexed_by<
			random_access<>,
			ordered_non_unique<member<t_employee,int,&t_employee::type>>
		>
> employee_set;

void print_out_by_name(const employee_set& es)
{
	const employee_set::nth_index<1>::type& name_index=es.get<1>();
	employee_set::nth_index_iterator<1>::type it=name_index.begin();
	std::cout << std::endl;
	while (it!=name_index.end())
	{
		if((*it).e)
			std::cout << (*it).type << "	" << (*it)->name << std::endl;
		else
			std::cout << 0 << std::endl;
		it++;
	}
}

void print_out_by_id(const employee_set& es)
{
	std::cout << std::endl;
	for (int i=0; i<es.size(); i++)
	{
		std::cout << i << ") ";
		if(es[i].e)
			std::cout << es[i].type << "	" << es[i]->name << std::endl;
		else
			std::cout << "*	NULL" << std::endl;
	}
}

void print_out_of_type(const employee_set& es,int type)
{
	employee_set::nth_index_iterator<1>::type it   = es.get<1>().lower_bound(type);
	employee_set::nth_index_iterator<1>::type itend= es.get<1>().upper_bound(type);
	std::cout << std::endl;
	while (it!=itend)
	{
		std::cout << (*it).type << "	" << (*it)->name << std::endl;
		it++;
	}
}

template<typename Tag,typename MultiIndexContainer>
void print_out_by(
	const MultiIndexContainer& s,
	Tag* =0
)
{
	const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
	get<Tag>(s);

	typedef typename MultiIndexContainer::value_type value_type;
}

BOOST_AUTO_TEST_SUITE(multiindex_test)

BOOST_AUTO_TEST_CASE(always_true)
{
	employee_set es;

	t_employee t1(1,0,"Joe",31);
	std::cout << t1->name << " of type " << t1.type << std::endl;

	es.push_back(t_employee(1,0,"Joe",31));
	es.push_back(t_employee(3,2,"John",40));
	es.push_back(t_employee(2,3,"Albert",20));
	es.push_back(t_employee(1,1,"Robert",27));
	es.push_back(t_employee(3,4,"John",57));

	print_out_by_id(es);

	print_out_by_name(es);

	print_out_of_type(es,1);
	print_out_of_type(es,2);
	print_out_of_type(es,3);

	es.replace(es.get<0>().begin() + 3,t_employee());

	print_out_by_id(es);

	print_out_by_name(es);

	print_out_of_type(es,1);
	print_out_of_type(es,2);
	print_out_of_type(es,3);

	es.replace(es.get<0>().begin() + 3,t_employee(1,1,"Robert is back",27));

	print_out_by_id(es);

	print_out_by_name(es);

	print_out_of_type(es,1);
	print_out_of_type(es,2);
	print_out_of_type(es,3);

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END() // multiindex_test