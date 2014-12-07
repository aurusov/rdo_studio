#ifndef _UTILS_STATIC_ASSERT_H_
#define _UTILS_STATIC_ASSERT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

//! \struct  rdo::CompileTimeError
//! \brief   Шаблонный класс для генерации ошибки компиляции
//! \details Используется через макрос \ref STATIC_ASSERT
//! \tparam  bool - если \b true, ошибки нет, если \b false - ошибка есть
template <bool>
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

} // namespace rdo

#endif // _UTILS_STATIC_ASSERT_H_
