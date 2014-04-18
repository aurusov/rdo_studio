/*!
  \copyright (c) RDO-Team, 2011
  \file      factory.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_FACTORY_H_
#define _UTILS_SMART_PTR_FACTORY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class intrusive_ptr;

template <class T>
class RefCounter;

//! Фабрика, используется для создания и удаления объектов
//! \tparam T - Тип создаваемого объекта
template <class T>
class Factory
{
friend class intrusive_ptr<T>;
friend class RefCounter<T>;
public:
	//! Создает объект класса Т
	//! \result Умный указатель на объект
	static intrusive_ptr<T> create();

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1>
	static intrusive_ptr<T> create(const P1& p1);

	//! Создает объект класса Т через шаблонный статический метод create класса создаваемого объекта
	//! \tparam T1 - тип, передаваемый в шаблонный конструтор
	//! \param  p1 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename T1, typename P1>
	static intrusive_ptr<T> delegate(const P1& p1);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \param p8 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8);

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \param p8 - параметр конструктора
	//! \param p9 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static intrusive_ptr<T> create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9);

private:
	static void destroy(T* object);
};

#define DECLARE_FACTORY(TYPE) friend class rdo::Factory<TYPE>;

} // namespace rdo

#endif // _UTILS_SMART_PTR_FACTORY_H_
