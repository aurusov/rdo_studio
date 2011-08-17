/*!
  \copyright (c) RDO-Team, 2011
  \file      namespace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     
  \indent    4T
*/

#ifndef _COMMON_NAMESPACE_H_
#define _COMMON_NAMESPACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

//! \namespace rdo
//! \brief     Базовое пространство имён
#define OPEN_RDO_NAMESPACE					namespace rdo {
#define CLOSE_RDO_NAMESPACE					};

//! \namespace rdoParse
//! \brief     Пространство имён парсера
#define OPEN_RDO_PARSE_NAMESPACE			namespace rdoParse {
#define CLOSE_RDO_PARSE_NAMESPACE			};

//! \namespace rdoRuntime
//! \brief     Пространство имён рантайма
#define OPEN_RDO_RUNTIME_NAMESPACE			namespace rdoRuntime {
#define CLOSE_RDO_RUNTIME_NAMESPACE			};

//! \namespace rdoSimulator
//! \brief     Пространство имён симулятора
#define OPEN_RDO_SIMULATOR_NAMESPACE		namespace rdoSimulator {
#define CLOSE_RDO_SIMULATOR_NAMESPACE		};

//! \namespace rdoAnimation
//! \brief     Пространство имён анимации
#define OPEN_RDO_ANIMATION_NAMESPACE		namespace rdoAnimation {
#define CLOSE_RDO_ANIMATION_NAMESPACE		};

#endif //! _COMMON_NAMESPACE_H_
