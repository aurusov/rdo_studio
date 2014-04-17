/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Романов Ярослав (robot.xet@gmail.com)
  \date      13.07.2009
  \brief     Тест для RDOValue
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOValue_Test
#include "utils/src/common/warning_disable.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

template <class F>
void testException(F binder)
{
	bool flag = false;
	try
	{
		binder();
	}
	catch(const RDOValueException&)
	{
		flag = true;
	}
	BOOST_CHECK(flag);
}

void testing(RDOValue value1, RDOValue value2)
{
	testException(boost::bind(&RDOValue::operator+=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator-=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator*=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator/=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator>,  &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator<,  &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator>=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator<=, &value1, boost::cref(value2)));
	testException(boost::bind(&RDOValue::operator==, &value1, boost::cref(value2)));
}

void compare(RDOValue value1, RDOValue value2)
{
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 <  value2);
	BOOST_CHECK(value2 >  value1);
	BOOST_CHECK(value1 <= value2);
	BOOST_CHECK(value2 >= value1);
}

template <class T1>
void compareOne(const T1& param1, const T1& param2)
{
	const T1 val1 = param1;
	const T1 val2 = param2;
	RDOValue value1(val1);
	RDOValue value2(val2);

	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 >  value2);
	BOOST_CHECK(value2 <  value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = val1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
}

template <class T1, class T2>
void compareValue(const T1& param1, const T2& param2, const RDOType::TypeID& type1, const RDOType::TypeID& type2)
{
	T1 val1 = param1;
	T2 val2 = param2;
	RDOValue value1(val1);
	RDOValue value2(val2);

	compare(value1, value2);

	value1 = value2;
	BOOST_CHECK(value1 == value2);

	value1 = val1;
	value2 = val2;
	value1 += value2;
	BOOST_CHECK(value1 == T1(param1 + param2));
	BOOST_CHECK(value1.typeID() == type1);
	value1 -= value2;
	BOOST_CHECK(value1 == param1);
	value1 *= value2;
	BOOST_CHECK(value1 == param1 * param2);
	value1 /= value2;
	BOOST_CHECK(value1 == param1);

	BOOST_CHECK(value1.typeID() == type2);
}

template <class T1, class T2>
void compareStr(const T1& param1, const T2& param2)
{
	T1 val1 = param1;
	T2 str1 = param2;
	RDOValue value1(val1);
	RDOValue value2(str1);

	testing(value1, value2);
}

template <class T1, class T2>
void compareChr(const T1& param1, const T2& param2)
{
	T1  val1 = param1;
	T2  ch1  = param2;
	RDOValue value1(val1);
	RDOValue value2(ch1 );

	value1 += value2;
	BOOST_CHECK(value1 == val1 + ch1);
	value1 -= value2;
	BOOST_CHECK(value1 == val1);
	value1 *= value2;
	BOOST_CHECK(value1 == val1 * ch1);
	value1 /= value2;
	BOOST_CHECK(value1 == val1);

	compare(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_String)
{
	const std::string str1 = "qqq";
	RDOValue value1(str1);
	BOOST_CHECK(value1.getString() == str1);
	BOOST_CHECK(value1.getAsString() == str1);

	RDOValue value2 = value1;
	BOOST_CHECK(value2.getString() == str1);
	BOOST_CHECK(value2.getAsString() == str1);
	BOOST_CHECK(value2 == value1);

	const std::string str2 = "aaa";
	value2 += str2;
	BOOST_CHECK(value2.getString() == str1 + str2);
	BOOST_CHECK(value2.getAsString() == str1 + str2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Arifmethic)
{
	const int val1 = 30;
	RDOValue value1(val1);
	BOOST_CHECK(value1.getInt      () == val1);
	BOOST_CHECK(value1.getUInt     () == std::size_t(val1));
	BOOST_CHECK(value1.getDouble   () == val1);
	BOOST_CHECK(value1.getAsString () == "30");
	BOOST_CHECK(value1.getEnumAsInt() == 30  );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == val1  );
	BOOST_CHECK(value2 == value1);

	const int val2 = 20;
	value1 += val2;
	BOOST_CHECK(value1 == val1 + val2);
	value1 = val1;
	value1 = value1 + val2;
	BOOST_CHECK(value1 == val1 + val2);

	const int val3 = 10;
	value1 -= val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);
	value1 += val3;
	value1 = value1 - val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);

	const int val4 = 2;
	value2 = value1;
	value2 *= val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);
	value2 /= val4;
	value2 = value2 * val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);

	const int val5 = 5;
	value2 = value1;
	value2 /= val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));
	value2 *= val5;
	value2 = value2 / val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Compare)
{
	compareOne<int>(30, 20);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Arifmethic)
{
	const std::size_t val1 = 30;
	RDOValue value1(val1);
	BOOST_CHECK(value1 == val1);
	BOOST_CHECK(value1.getInt      () == int(val1));
	BOOST_CHECK(value1.getUInt     () == val1);
	BOOST_CHECK(value1.getDouble   () == val1);
	BOOST_CHECK(value1.getAsString () == "30");
	BOOST_CHECK(value1.getEnumAsInt() == 30  );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == val1  );
	BOOST_CHECK(value2 == value1);

	const std::size_t val2 = 20;
	value1 += val2;
	BOOST_CHECK(value1 == val1 + val2);
	value1 = val1;
	value1 = value1 + val2;
	BOOST_CHECK(value1 == val1 + val2);

	const std::size_t val3 = 10;
	value1 -= val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);
	value1 += val3;
	value1 = value1 - val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);

	const std::size_t val4 = 2;
	value2 = value1;
	value2 *= val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);
	value2 /= val4;
	value2 = value2 * val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);

	const std::size_t val5 = 5;
	value2 = value1;
	value2 /= val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));
	value2 *= val5;
	value2 = value2 / val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Compare)
{
	compareOne<std::size_t>(30, 20);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_Arifmethic)
{
	const double doub1 = 30.2;
	RDOValue value1(doub1);
	BOOST_CHECK(value1 == doub1);
	BOOST_CHECK(value1.getInt      () == 30    );
	BOOST_CHECK(value1.getUInt     () == 30    );
	BOOST_CHECK(value1.getDouble   () == doub1 );
	BOOST_CHECK(value1.getAsString () == "30.2");
	BOOST_CHECK(value1.getEnumAsInt() == 30    );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == doub1);
	BOOST_CHECK(value2 == value1);

	const double doub2 = 20.5;
	value1 += doub2;
	BOOST_CHECK(value1 == doub1 + doub2);
	value1 = doub1;
	value1 = value1 + doub2;
	BOOST_CHECK(value1 == doub1 + doub2);

	const double doub3 = 10.3;
	value1 -= doub3;
	BOOST_CHECK(value1 == doub1 + doub2 - doub3);
	value1 += doub3;
	value1 = value1 - doub3;
	BOOST_CHECK(value1 == doub1 + doub2 - doub3);

	const double doub4 = 2.5;
	value2 = value1;
	value2 *= doub4;
	BOOST_CHECK(value2 == (doub1 + doub2 - doub3) * doub4);
	value2 /= doub4;
	value2 = value2 * doub4;
	BOOST_CHECK(value2 == (doub1 + doub2 - doub3) * doub4);

	const double doub5 = 5;
	value2 = value1;
	value2 /= doub5;
	BOOST_CHECK(value2 == ((doub1 + doub2 - doub3) / doub5));
	value2 *= doub5;
	value2 = value2 / doub5;
	BOOST_CHECK(value2 == ((doub1 + doub2 - doub3) / doub5));

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_Compare)
{
	compareOne<double>(30.2, 20.5);
}

BOOST_AUTO_TEST_CASE(RDOValue_Bool)
{
	bool bool1 = true;
	bool bool2 = false;
	bool bool3 = true;
	bool bool4 = false;

	RDOValue value1(bool1);
	RDOValue value2(bool2);
	RDOValue value3(bool3);
	RDOValue value4(bool4);

	BOOST_CHECK(value1.getBool     ());
	BOOST_CHECK(value1.getInt      () == 1);
	BOOST_CHECK(value1.getUInt     () == 1);
	BOOST_CHECK(value1.getEnumAsInt() == 1);
	BOOST_CHECK(value1.getDouble   () == 1);
	BOOST_CHECK(!value2);
	BOOST_CHECK(  value2 == value4 );
	BOOST_CHECK(  value1 != value2 );
	BOOST_CHECK(!(value1 == value2));
	BOOST_CHECK(  value1 && value3 );
	BOOST_CHECK(!(value2 && value3));
	BOOST_CHECK(  value1 || value2 );
	BOOST_CHECK(!(value2 || value4));
}

BOOST_AUTO_TEST_CASE(RDOValue_Char)
{
	const char ch1 = 'a';
	const char ch2 = 'b';
	RDOValue value1 = ch1;
	RDOValue value2 = ch2;
	BOOST_CHECK(value1 == ch1   );
	BOOST_CHECK(value1 <  value2);
	BOOST_CHECK(value2 >  value1);
	BOOST_CHECK(value1 != value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Enum)
{
	LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
	BOOST_CHECK(pEnum);
	BOOST_CHECK(pEnum->empty());
	pEnum->add("test0");
	pEnum->add("test1");
	pEnum->add("test2");
	pEnum->add("test3");
	BOOST_CHECK(pEnum->findEnum("test1") == 1);
	BOOST_CHECK(pEnum->exist   ("test3"));

	RDOValue value(pEnum);
	BOOST_CHECK(value.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value.getEnum     () == pEnum);
	BOOST_CHECK(value.getInt      () == 0);
	BOOST_CHECK(value.getDouble   () == 0);
	BOOST_CHECK(value.getEnumAsInt() == 0);
	BOOST_CHECK(!value.getAsBool  ());
	BOOST_CHECK(value.getAsString () == "test0");

	RDOValue value1(pEnum, "test2");
	BOOST_CHECK(value1.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value1.getEnum     () == pEnum);
	BOOST_CHECK(value1.getInt      () == 2);
	BOOST_CHECK(value1.getDouble   () == 2);
	BOOST_CHECK(value1.getEnumAsInt() == 2);
	BOOST_CHECK(value1.getAsBool   ());
	BOOST_CHECK(value1.getAsString () == "test2");

	RDOValue value2(pEnum, 2);
	BOOST_CHECK(value2.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value2.getEnum     () == pEnum);
	BOOST_CHECK(value2.getInt      () == 2);
	BOOST_CHECK(value2.getDouble   () == 2);
	BOOST_CHECK(value2.getEnumAsInt() == 2);
	BOOST_CHECK(value2.getAsBool   ());
	BOOST_CHECK(value2.getAsString () == "test2");

}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Ruint)
{
	compareValue<int, std::size_t>(10, 15, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Rsint)
{
	compareValue<std::size_t, int>(10, 15, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Double)
{
	compareValue<int, double>(10, 15.2, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_rsint)
{
	compareValue<double, int>(10.2, 15, RDOType::t_real, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Double)
{
	compareValue<std::size_t, double>(10, 15.2, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_ruint)
{
	compareValue<double, std::size_t>(10.2, 15, RDOType::t_real, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_String)
{
	compareStr<int, std::string>(10, "abc");
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_String)
{
	compareStr<std::size_t, std::string>(10, "abc");
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_String)
{
	compareStr<double, std::string>(10, "abc");
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Rsint)
{
	compareChr<int, char>(10, 'a');
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Ruint)
{
	compareChr<std::size_t, char>(10, 'a');
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Double)
{
	compareChr<double, char>(10, 'a');
}

BOOST_AUTO_TEST_CASE(RDOValue_String_Char)
{
	compareStr<char, std::string>('a', "abc");
}

BOOST_AUTO_TEST_CASE(RDOValue_Identificator)
{
	const std::string str = "abc";
	RDOValue value1(str, g_identificator);
	BOOST_CHECK(value1.typeID() == RDOType::t_identificator);
	std::string iden = value1.getIdentificator();
	BOOST_CHECK(iden == str);
	iden = value1.getAsString();
	BOOST_CHECK(iden == str);

	const std::string str2 ="dba";
	RDOValue value2(str2, g_identificator);
	BOOST_CHECK(value1 != value2);

	RDOValue value3(str, g_identificator);
	BOOST_CHECK(value1 == value3);
}

template <class T1>
void testUndef(const T1& param1)
{
	T1 val = param1;
	RDOValue value(val);
	BOOST_CHECK(value.isUndefined() == false);
}

BOOST_AUTO_TEST_CASE(RDOValue_Undefined)
{
	testUndef<int>(10);
	testUndef<std::size_t>(10);
	testUndef<double>(10.5);
	testUndef<std::string>("abc");
	testUndef<char>('a');
	testUndef<bool>(true);

	int val1 = 10;
	RDOValue value1(val1);
	BOOST_CHECK(value1);

	BOOST_CHECK(value1.isUndefined() == false);
	value1.setUndefined(true);
	BOOST_CHECK(value1.isUndefined() == true);

	RDOValue value2(value1);
	BOOST_CHECK(value2.isUndefined() == true);
}

namespace
{

rdo::runtime::LPRDOResource createSimpleResource(
	const rdo::runtime::LPRDORuntime            runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType&        type,
	std::size_t resource_id,
	std::size_t type_id,
	bool trace,
	bool temporary)
{
	return rdo::Factory<rdo::runtime::RDOResource>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

}

BOOST_AUTO_TEST_CASE(RDOValue_Resource)
{
	Error error;
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(&error);
	BOOST_CHECK(pRuntime);

	LPRDOResourceTypeList pResourceType = rdo::Factory<RDOResourceTypeList>::create(1, pRuntime);
	BOOST_CHECK(pResourceType);
	pResourceType->setFactoryMethod(boost::bind(&createSimpleResource, _1, _2, _3, _4, _5, _6, _7));

	LPIResourceType pResourceFactory = pResourceType.interface_cast<IResourceType>();
	BOOST_CHECK(pResourceFactory);

	std::vector<RDOValue> paramList;
	paramList.push_back(RDOValue(1));
	paramList.push_back(RDOValue(2.2));
	paramList.push_back(RDOValue("3"));

	LPRDOResource pResource = pResourceFactory->createRes(pRuntime, pRuntime->getResourceId(), paramList, true, true);
	BOOST_CHECK(pResource);

	RDOValue value1(pResourceType, pResource);

	LPRDOResourceTypeList pType = value1.type().object_dynamic_cast<RDOResourceTypeList>();
	BOOST_CHECK(pType);

	LPRDOResource pResource1 = value1.getPointerByType<RDOResourceTypeList>();

	pRuntime   = NULL;
	value1     = RDOValue();
	pResource1 = NULL;
	pResourceType->eraseRes(pResource);

	BOOST_CHECK(pResource.owner());

	pResource = NULL;
}

BOOST_AUTO_TEST_SUITE_END() // RDOValue_Test

CLOSE_RDO_RUNTIME_NAMESPACE
