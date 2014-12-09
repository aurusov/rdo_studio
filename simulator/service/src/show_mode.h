#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace service {
namespace simulation {

//! Режимы анимации
enum ShowMode
{
    SM_NoShow,    // Без анимации
    SM_Animation, // С анимацией
    SM_Monitor    // Анимация и модель на паузе
};

}}} // namespace rdo::service::simulation
