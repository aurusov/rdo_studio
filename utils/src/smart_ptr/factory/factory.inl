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
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
inline intrusive_ptr<T> Factory<T>::create()
{
	T* pObject = new T();
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1)
{
	T* pObject = new T(p1);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename T1, typename P1>
inline intrusive_ptr<T> Factory<T>::delegate(const P1& p1)
{
	return T::template create<T1>(p1);
}

template <class T>
template <typename P1, typename P2>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2)
{
	T* pObject = new T(p1, p2);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3)
{
	T* pObject = new T(p1, p2, p3);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	T* pObject = new T(p1, p2, p3, p4);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	T* pObject = new T(p1, p2, p3, p4, p5);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	T* pObject = new T(p1, p2, p3, p4, p5, p6);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	T* pObject = new T(p1, p2, p3, p4, p5, p6, p7);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	T* pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8);
	return intrusive_ptr<T>(pObject);
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline intrusive_ptr<T> Factory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9)
{
	T* pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	return intrusive_ptr<T>(pObject);
}

template <class T>
inline void Factory<T>::destroy(T* object)
{
	delete object;
}

} // namespace rdo
