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
