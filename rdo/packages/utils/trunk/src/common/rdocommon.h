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
#include "utils/src/common/namespace.h"
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/model_objects.h"
#include "utils/src/stream/rdostream.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

OPEN_RDO_NAMESPACE

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

	//! Переводит значение в строку
	//! \tparam T     - тип значения
	//! \param  value - значение
	//! \result Строковое представление значения
	template< class T >
	inline tstring toString(T value)
	{
		rdo::ostringstream str;
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
			std::vector<T>::push_back(item);
		}

		//! Добавляет в контейнер новый эелемент, более удобный аналог std::vector::push_back()
		//! \param item - Элемент контейнера
		//! \result Возвращает сам контейнер, чтобы эту же операцию можно было вызвать снова
		REF(vector) operator() (CREF(T) item)
		{
			std::vector<T>::push_back(item);
			return *this;
		}
	};

	//! Округляет вещественное число до целого
	//! \param  value - вещественное значение
	//! \result Результат округления
	int roundDouble(double val);

CLOSE_RDO_NAMESPACE

#include "utils/src/common/rdocommon.inl"

#endif // _UTILS_RDOCOMMON_H_
