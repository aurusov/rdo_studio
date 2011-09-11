RDOValue integerVal1(1);
RDOValue integerVal2(2);
RDOValue integerVal3(3);
RDOValue integerVal4(4);
RDOValue integerVal5(5);
RDOValue integerVal6(6);

RDOArrayType atype1(NULL, g_int);
RDOArrayValue avalue1(atype1);
avalue1.insertItem(integerVal1);
avalue1.insertItem(integerVal2);
avalue1.insertItem(integerVal3);
RDOValue arrayVal1(avalue1);

RDOArrayType atype2(NULL, g_int);
RDOArrayValue avalue2(atype2);
avalue2.insertItem(integerVal4);
avalue2.insertItem(integerVal5);
avalue2.insertItem(integerVal6);
RDOValue arrayVal2(avalue2);