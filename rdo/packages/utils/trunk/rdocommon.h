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
#include "simulator/runtime/namespace.h"
#include "simulator/service/namespace.h"
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

OPEN_RDO_RUNTIME_NAMESPACE
	//! Режимы моделирования
	enum RunTimeMode
	{
		RTM_MaxSpeed,   //!< Максимальная производительность
		RTM_Jump,       //!< Дискретная имитация
		RTM_Sync,       //!< Синхронная имитация
		RTM_Pause,      //!< Режим паузы
		RTM_BreakPoint  //!< Режим точки остановки
	};
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_SIMULATOR_NAMESPACE
	//! Режимы анимации
	enum ShowMode
	{
		SM_NoShow,    //!< Без анимации
		SM_Animation, //!< С анимацией
		SM_Monitor    //!< Анимация и модель на паузе
	};

	//! Код окончания компиляции или прогона модели
	enum RDOExitCode
	{
		EC_OK = 0,        //!< Модель скомпилировалась или завершилась без ошибок
		EC_ParserError,   //!< Компиляция завершилась с ошибкой
		EC_RunTimeError,  //!< Прогон модели завершился с ошибкой
		EC_UserBreak,     //!< Модель остановлена пользователем
		EC_ModelNotFound, //!< Файл прогона модели не найден
		EC_NoMoreEvents   //!< Модель завершилась потому, что больше нечего моделировать
	};

	//! Синтаксическая ошибка
	struct RDOSyntaxError
	{
		//! \brief   Код ошибки
		//! \details Сейчас вместо кода ошибки используется текстовое сообщение об ошибке
		enum ErrorCode
		{
			UNKNOWN = 1, //!< Неизвестная ошибка
		};

		ErrorCode                     m_code;    //!< Код ошибки
		tstring                       m_message; //!< Сообщение об ошибке
		rdoModelObjects::RDOFileType  m_file;    //!< Файл, в котором найдена ошибка
		ruint                         m_line;    //!< Номер строки с ошибкой
		ruint                         m_pos;     //!< Позиция ошибки в строке
		rbool                         m_warning; //!< Признак предупреждения (\b true - предупреждение, \b false - ошибка)

		//! Конструктор ошибки
		//! \param code    - код ошибки
		//! \param message - сообщение об ошибке
		//! \param file    - файл, в котором найдена ошибка
		//! \param line    - номер строки с ошибкой
		//! \param pos     - позиция ошибки в строке
		//! \param warning - признак предупреждения, может отсутствовать. Значение по умолчанию \b false
		RDOSyntaxError(ErrorCode code, CREF(tstring) message, ruint line, ruint pos, rdoModelObjects::RDOFileType file, rbool warning = false)
			: m_code   (code   )
			, m_message(message)
			, m_line   (line   )
			, m_pos    (pos    )
			, m_file   (file   )
			, m_warning(warning)
		{}
	};
CLOSE_RDO_SIMULATOR_NAMESPACE

#include "utils/rdocommon.inl"

#endif // _UTILS_RDOCOMMON_H_
