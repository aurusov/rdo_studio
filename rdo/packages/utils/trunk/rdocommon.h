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
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdarg.h>
#include <vector>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

OPEN_RDO_NAMESPACE

	//! Определяет минимальное значение
	//! \tparam T      - тип сравниваемых переменных
	//! \param  v1, v2 - значения, из которых выбирается минимальное
	//! \result Минимальное значение
	template< class T >
	T rmin(T v1, T v2)
	{
		return v1 <= v2 ? v1 : v2;
	}

	//! Определяет максимальное значение
	//! \tparam T      - тип сравниваемых переменных
	//! \param  v1, v2 - значения, из которых выбирается максимальное
	//! \result Максимальное значение
	template< class T >
	T rmax(T v1, T v2)
	{
		return v1 >= v2 ? v1 : v2;
	}

	//! Формирование строки по формату, аналогично <a href="http://ru.wikipedia.org/wiki/Printf">printf</a>
	//! \param  str - исходная строка, которая может содержать символы форматирования
	//! \param  ... - произвольный набор параметров
	//! \result Отформатированная строчка
	tstring format(CPTR(tchar) str, ...);

	//! Формирование строки по формату
	//! \param  str    - исходная строка, которая может содержать символы форматирования
	//! \param  params - определенный по формату набор параметров (вытащенный по формату из стека)
	//! \result Отформатированная строчка
	tstring format(CPTR(tchar) str, REF(va_list) params);

#ifdef COMPILER_VISUAL_STUDIO
	//! Формирование строки по формату, аналогично <a href="http://ru.wikipedia.org/wiki/Printf">printf</a>
	//! \param  resource - номер строкового ресурса, который может содержать символы форматирования
	//! \param  ...      - произвольный набор параметров
	//! \result Отформатированная строчка
	tstring format(ruint resource, ...);

	//! Формирование строки по формату
	//! \param  resource - номер строкового ресурса, который может содержать символы форматирования
	//! \param  params   - определенный по формату набор параметров (вытащенный по формату из стека)
	//! \result Отформатированная строчка
	tstring format(ruint resource, REF(va_list) params);
#endif // COMPILER_VISUAL_STUDIO

	//! Перевод строки в нижний регистр
	//! \param[in, out] str - изменяемая строка
	//! \result Переведенная в нижний регистр строка
	void toLower  (REF(tstring) str);

	//! Отбрасывает пустоты (пробел, табуляция, перевод строки) в начале и конце строки
	//! \param[in, out] str - изменяемая строка
	//! \result Строка без пустот в начале и конце
	void trim     (REF(tstring) str);

	//! Отбрасывает пустоту (пробел, табуляция, перевод строки) в начале строки
	//! \param[in, out] str - изменяемая строка
	//! \result Строка без пустот в начале
	void trimLeft (REF(tstring) str);

	//! Отбрасывает пустоту (пробел, табуляция, перевод строки) в конце строки
	//! \param[in, out] str - изменяемая строка
	//! \result Строка без пустот в конце
	void trimRight(REF(tstring) str);

	//! Получение пути файла по его имени
	//! \param fileName - полное имя файла
	//! \result Путь к файлу, заканчивающийся '/'
	tstring extractFilePath(CREF(tstring) fileName);

	//! Переводит значение в строку
	//! \tparam T     - тип значения
	//! \param  value - значение
	//! \result Строковое представление значения
	template< class T >
	inline tstring toString(T value)
	{
		std::ostringstream str;
		str << value;
		return str.str();
	}

	//! Удаляет все элементы контейнера, начиная с последнего
	//! \tparam         T         - тип контейнера
	//! \param[in, out] container - контейнер
	template<class T>
	void deleteAllObjects(REF(T) container)
	{
		typename T::reverse_iterator it = container.rbegin();
		while (it != container.rend())
		{
			delete *it;
			++it;
		}
		container.clear();
	}

	//! \class  vector
	//! \tparam T - Тип хранимого элемента
	//! \brief    Вектор
	//! \details  Потомок от std::vector, с парой удобных методов. Пример:
	//! \code
	//! rdo::vector<ruint> container;
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
		vector(CREF(T) item)
		{
			push_back(item);
		}

		//! Добавляет в контейнер новый эелемент, более удобный аналог std::vector::push_back()
		//! \param item - Элемент контейнера
		//! \result Возвращает сам контейнер, чтобы эту же операцию можно было вызвать снова
		REF(vector) operator() (CREF(T) item)
		{
			push_back(item);
			return *this;
		}
	};
CLOSE_RDO_NAMESPACE

#include "utils/rdocommon.inl"

#endif // _UTILS_RDOCOMMON_H_
