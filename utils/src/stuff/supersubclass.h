/*!
  \copyright (c) RDO-Team, 2011
  \file      supersubclass.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.03.2010
  \brief     основан на классе Conversion из Александреску
  \indent    4T
*/

#ifndef _UTILS_SUPERSUBCLASS_H_
#define _UTILS_SUPERSUBCLASS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <class T, class U>
class SuperSubClass
{
private:
	typedef char Small;
	class        Big    { char dummy[2]; };

	static Small Test (const U&);
	static Big   Test (...);
	static T     MakeT();

public:
	enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
	enum { same   = false };
};

template <class T>
class SuperSubClass<T, T>
{
public:
	enum { exists = true };
	enum { same   = true };
};

#define SUPERSUBCLASS(T, U) (SuperSubClass<const U*, const T*>::exists)

} // namespace rdo

#endif // _UTILS_SUPERSUBCLASS_H_
