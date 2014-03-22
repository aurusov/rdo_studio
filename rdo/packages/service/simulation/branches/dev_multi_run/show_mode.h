/*!
  \copyright (c) RDO-Team, 2012
  \file      show_mode.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     Режимы анимации
  \indent    4T
*/

#ifndef _SIMULATOR_SERVICE_SHOW_MODE_H_
#define _SIMULATOR_SERVICE_SHOW_MODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace service {
namespace simulation {

//! Режимы анимации
enum ShowMode
{
	SM_NoShow,    //!< Без анимации
	SM_Animation, //!< С анимацией
	SM_Monitor    //!< Анимация и модель на паузе
};

}}} // namespace rdo::service::simulation

#endif // _SIMULATOR_SERVICE_SHOW_MODE_H_
