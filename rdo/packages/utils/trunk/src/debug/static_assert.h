/*!
  \copyright (c) RDO-Team, 2011
  \file      static_assert.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.03.2010
  \brief     Основан на классе CompileTimeError из <a href="http://lib.mexmat.ru/books/7498">Александреску</a>
  \indent    4T
*/

#ifndef _UTILS_STATIC_ASSERT_H_
#define _UTILS_STATIC_ASSERT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

//! \struct  rdo::CompileTimeError
//! \brief   Шаблонный класс для генерации ошибки компиляции
//! \details Используется через макрос \ref STATIC_ASSERT
//! \tparam  rbool - если \b true, ошибки нет, если \b false - ошибка есть
template <rbool>
struct CompileTimeError;

//! Специализация шаблонного класса \ref rdo::CompileTimeError для описание ситуации \b отсутствия ошибки компиляции
template <>
struct CompileTimeError<true> {};

//! Выбрасывает ASSERT на этапе компиляции, когда компилятор доходит до этого макроса, т.е. формирует ошибку компиляции. Пример:
//! \code
//! STATIC_ASSERT(Some_error);
//! \endcode
//! \param expr - текст ошибки, указывается без пробелов
#define STATIC_ASSERT(expr)                 \
	rdo::CompileTimeError<((expr) != 0)>();

CLOSE_RDO_NAMESPACE

#endif // _UTILS_STATIC_ASSERT_H_
