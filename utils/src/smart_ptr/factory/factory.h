#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class intrusive_ptr;

template <class T>
class RefCounter;

template <class T>
class Factory
{
friend class intrusive_ptr<T>;
friend class RefCounter<T>;
public:
	static intrusive_ptr<T> create();

	template <typename P1>
	static intrusive_ptr<T> create(const P1& p1);

	template <typename T1, typename P1>
	static intrusive_ptr<T> delegate(const P1& p1);

	template <typename P1, typename P2>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2);

	template <typename P1, typename P2, typename P3>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3);

	template <typename P1, typename P2, typename P3, typename P4>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4);

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9);

private:
	static void destroy(T* object);
};

#define DECLARE_FACTORY(TYPE) friend class rdo::Factory<TYPE>;

} // namespace rdo
