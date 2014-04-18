/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocommon.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.06.2009
  \brief     Общесистемные функции и типы
  \indent    4T
*/

#ifndef _UTILS_RDOCOMMON_H_
#define _UTILS_RDOCOMMON_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdarg.h>
#include <vector>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

namespace rdo {

	//! Формирование строки по формату, аналогично <a href="http://ru.wikipedia.org/wiki/Printf">printf</a>
	//! \param  str - исходная строка, которая может содержать символы форматирования
	//! \param  ... - произвольный набор параметров
	//! \result Отформатированная строчка
	std::string format(const char* str, ...);

	//! Формирование строки по формату
	//! \param  str    - исходная строка, которая может содержать символы форматирования
	//! \param  params - определенный по формату набор параметров (вытащенный по формату из стека)
	//! \result Отформатированная строчка
	std::string format(const char* str, va_list& params);

	//! Переводит значение в строку
	//! \tparam T     - тип значения
	//! \param  value - значение
	//! \result Строковое представление значения
	template< class T >
	inline std::string toString(const T& value)
	{
		std::stringstream str;
		str << value;
		return str.str();
	}

	//! \class  vector
	//! \tparam T - Тип хранимого элемента
	//! \brief    Вектор
	//! \details  Потомок от std::vector, с парой удобных методов. Пример:
	//! \code
	//! rdo::vector<std::size_t> container;
	//! container(1)(3)(5)(2);        // Размещает в контейнере 4-ре элемента
	//! \endcode
	template <class T>
	class vector: public std::vector<T>
	{
	public:
		//! \details Создаёт пустой контейнер
		vector()
		{}

		//! Создаёт пустой контейнер и помещает в него \ref item
		//! \param item - Элемент контейнера
		vector(const T& item)
		{
			std::vector<T>::push_back(item);
		}

		//! Добавляет в контейнер новый эелемент, более удобный аналог std::vector::push_back()
		//! \param item - Элемент контейнера
		//! \result Возвращает сам контейнер, чтобы эту же операцию можно было вызвать снова
		vector& operator() (const T& item)
		{
			std::vector<T>::push_back(item);
			return *this;
		}
	};

	//! Округляет вещественное число до целого
	//! \param  value - вещественное значение
	//! \result Результат округления
	int roundDouble(double val);

} // namespace rdo

#include "utils/src/common/rdocommon.inl"

#endif // _UTILS_RDOCOMMON_H_
