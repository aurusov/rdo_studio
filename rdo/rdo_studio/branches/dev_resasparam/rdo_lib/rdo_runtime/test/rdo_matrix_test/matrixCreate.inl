RDOValue integerVal1(1);
RDOValue integerVal2(2);
RDOValue integerVal3(3);
RDOValue integerVal4(4);
RDOValue integerVal5(5);
RDOValue integerVal6(6);

RDOMatrixType mtype1(NULL, g_int);
RDOMatrixValue mvalue1(mtype1);
mvalue1.insertItem(integerVal1);
mvalue1.insertItem(integerVal2);
mvalue1.insertItem(integerVal3);
RDOValue matrixVal1(mvalue1);

RDOMatrixType mtype2(NULL, g_int);
RDOMatrixValue mvalue2(mtype2);
mvalue2.insertItem(integerVal4);
mvalue2.insertItem(integerVal5);
mvalue2.insertItem(integerVal6);
RDOValue matrixVal2(mvalue2);