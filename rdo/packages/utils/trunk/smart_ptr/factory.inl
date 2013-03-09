/*!
  \copyright (c) RDO-Team, 2011
  \file      factory.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template <class T>
inline intrusive_ptr<T> Factory<T>::create()
{
	PTR(T) pObject = new T();
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1)
{
	PTR(T) pObject = new T(p1);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename T1, typename P1>
inline intrusive_ptr<T> Factory<T>::delegate(CREF(P1) p1)
{
	return T::template create<T1>(p1);
}

template <class T>
template <typename P1, typename P2>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2)
{
	PTR(T) pObject = new T(p1, p2);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
{
	PTR(T) pObject = new T(p1, p2, p3);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
{
	PTR(T) pObject = new T(p1, p2, p3, p4);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
{
	PTR(T) pObject = new T(p1, p2, p3, p4, p5);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
{
	PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
{
	PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8)
{
	PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline intrusive_ptr<T> Factory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8, CREF(P9) p9)
{
	PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	return intrusive_ptr<T>(pObject);
}

template <class T>
FORCE_INLINE void Factory<T>::destroy(PTR(T) object)
{
	delete object;
}

CLOSE_RDO_NAMESPACE
