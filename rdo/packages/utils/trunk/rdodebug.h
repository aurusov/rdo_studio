/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodebug.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Декларация макросов для отладки
  \indent    4T
*/

#ifndef _UTILS_RDODEBUG_H_
#define _UTILS_RDODEBUG_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	#include <iostream>
	#include <assert.h>
#endif // COMPILER_GCC
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef TRACE
#undef TRACE
#endif
#ifdef TRACE1
#undef TRACE1
#endif
#ifdef TRACE2
#undef TRACE2
#endif
#ifdef TRACE3
#undef TRACE3
#endif
#ifdef ASSERT
#undef ASSERT
#endif

#ifdef _DEBUG

#ifdef COMPILER_VISUAL_STUDIO

#define TRACE(A)                   ::OutputDebugString(A);
#define TRACE1(A, P1)              ::OutputDebugString(rdo::format(A, P1).c_str());
#define TRACE2(A, P1, P2)          ::OutputDebugString(rdo::format(A, P1, P2).c_str());
#define TRACE3(A, P1, P2, P3)      ::OutputDebugString(rdo::format(A, P1, P2, P3).c_str());
#define TRACE4(A, P1, P2, P3, P4)  ::OutputDebugString(rdo::format(A, P1, P2, P3, P4).c_str());

#define ASSERT(A) \
	if (!(A)) \
	{ \
		::OutputDebugString(rdo::format("ASSERT: line %d, source '%s', condition '%s'\n", __LINE__, __FILE__, #A).c_str()); \
		::DebugBreak(); \
	}

template<typename type>
type identity_NEVER_REACH_HERE(type value)
{
	return value;
}

#define NEVER_REACH_HERE ASSERT(identity_NEVER_REACH_HERE(false))

#else // not COMPILER_VISUAL_STUDIO

void NewOutputDebugString(CREF(tstring) message)
{
	std::cerr << message << std::endl;
}

#define TRACE(A)                   NewOutputDebugString(A);
#define TRACE1(A, P1)              NewOutputDebugString(rdo::format(A, P1).c_str());
#define TRACE2(A, P1, P2)          NewOutputDebugString(rdo::format(A, P1, P2).c_str());
#define TRACE3(A, P1, P2, P3)      NewOutputDebugString(rdo::format(A, P1, P2, P3).c_str());
#define TRACE4(A, P1, P2, P3, P4)  NewOutputDebugString(rdo::format(A, P1, P2, P3, P4).c_str());
#define ASSERT(A)                  assert(A);

#define NEVER_REACH_HERE ASSERT(false)

#endif // COMPILER_VISUAL_STUDIO

#else // not _DEBUG

#define  TRACE(A)
#define TRACE1(A, P1)
#define TRACE2(A, P1, P2)
#define TRACE3(A, P1, P2, P3)
#define TRACE4(A, P1, P2, P3, P4)
#define ASSERT(A)
#define NEVER_REACH_HERE

#endif // _DEBUG

//! \def TRACE(A)
//! Выводит сообщение в окно Output Visial Studio. В режиме компиляции \b release удаляется из кода.
//! \param A - строка вывода

//! \def TRACE1(A, P1)
//! Выводит сообщение в окно Output Visial Studio, принимает один параметр для формирования итоговой строки. В режиме компиляции \b release удаляется из кода.
//! \param A  - строка вывода, должна содержать форматку в стиле <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> для одного параметра
//! \param P1 - параметр для вставки в результирующую строку

//! \def TRACE2(A, P1, P2)
//! Выводит сообщение в окно Output Visial Studio, принимает два параметра для формирования итоговой строки. В режиме компиляции \b release удаляется из кода.
//! \param A      - строка вывода, должна содержать форматку в стиле <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> для двух параметров
//! \param P1, P2 - параметры для вставки в результирующую строку

//! \def TRACE3(A, P1, P2, P3)
//! Выводит сообщение в окно Output Visial Studio, принимает три параметра для формирования итоговой строки. В режиме компиляции \b release удаляется из кода.
//! \param A          - строка вывода, должна содержать форматку в стиле <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> для трёх параметров
//! \param P1, P2, P3 - параметры для вставки в результирующую строку

//! \def ASSERT(A)
//! Если значение A равно \b false, то выводит сообщение об ошибке в окно Output Visial Studio и роняет приложение. В режиме компиляции \b release удаляется из кода, т.е. <b>нельзя внутри мароса располагать вызов функции или другой важный код</b>. Пример:
//! \code
//! LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
//! ASSERT(pThreadProxy);
//! // Значение указателя pThreadProxy не равно нулю, можно его использовать
//! pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
//! \endcode
//! \param A - Проверяемое значение

//! \def NEVER_REACH_HERE
//! Указывается в тех местах кода, до которых не должно дойти дело при работе программы. Аналог ASSERT(false). В режиме компиляции \b release удаляется из кода.

#endif // _UTILS_RDODEBUG_H_
