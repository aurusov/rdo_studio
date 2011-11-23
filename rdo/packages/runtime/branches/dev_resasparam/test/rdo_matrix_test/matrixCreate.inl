/*!
  \copyright (c) RDO-Team, 2011
  \file      matrixCreate.inl
  \authors   Чирков Михаил
  \date      01.10.2010
  \brief     Тест матриц
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

rdoRuntime::LPRDOMatrixType mtype1 = rdo::Factory<rdoRuntime::RDOMatrixType>::create(rdoRuntime::g_int);
rdoRuntime::RDOMatrixValue mvalue1(mtype1);
mvalue1.insertItem(integerVal1);
mvalue1.insertItem(integerVal2);
mvalue1.insertItem(integerVal3);
rdoRuntime::RDOValue matrixVal1(mvalue1);

rdoRuntime::LPRDOMatrixType mtype2 = rdo::Factory<rdoRuntime::RDOMatrixType>::create(rdoRuntime::g_int);
rdoRuntime::RDOMatrixValue mvalue2(mtype2);
mvalue2.insertItem(integerVal4);
mvalue2.insertItem(integerVal5);
mvalue2.insertItem(integerVal6);
rdoRuntime::RDOValue matrixVal2(mvalue2);
