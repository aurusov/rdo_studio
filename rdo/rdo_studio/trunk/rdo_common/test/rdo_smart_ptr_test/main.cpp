// ====================================================================== INCLUDES
#include <vector>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "../../rdosmart_ptr.h"
// ===============================================================================

class MyClass
{
public:
	ruint i;
};

void main()
{
	rdo::smart_ptr<MyClass> obj(rdo::Factory<MyClass>::create());
	obj->i = 10;
	int i = 1;
}
