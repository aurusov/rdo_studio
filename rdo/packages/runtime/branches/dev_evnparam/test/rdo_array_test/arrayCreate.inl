/*!
  \copyright (c) RDO-Team, 2011
  \file      arrayCreate.inl
  \authors   Чирков Михаил
  \date      01.10.2010
  \brief     Тест массивов
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

rdoRuntime::RDOValue integerVal1(1);
rdoRuntime::RDOValue integerVal2(2);
rdoRuntime::RDOValue integerVal3(3);
rdoRuntime::RDOValue integerVal4(4);
rdoRuntime::RDOValue integerVal5(5);
rdoRuntime::RDOValue integerVal6(6);

rdoRuntime::LPRDOArrayType atype1 = rdo::Factory<rdoRuntime::RDOArrayType>::create(rdoRuntime::g_int);
rdoRuntime::RDOArrayValue avalue1(atype1);
avalue1.insertItem(integerVal1);
avalue1.insertItem(integerVal2);
avalue1.insertItem(integerVal3);
rdoRuntime::RDOValue arrayVal1(avalue1);

rdoRuntime::LPRDOArrayType atype2 = rdo::Factory<rdoRuntime::RDOArrayType>::create(rdoRuntime::g_int);
rdoRuntime::RDOArrayValue avalue2(atype2);
avalue2.insertItem(integerVal4);
avalue2.insertItem(integerVal5);
avalue2.insertItem(integerVal6);
rdoRuntime::RDOValue arrayVal2(avalue2);
