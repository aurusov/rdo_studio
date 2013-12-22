/*!
  \copyright (c) RDO-Team, 2011
  \file      keyboard.h
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      16.05.2007
  \brief     keyboard virtual codes definitions
  \indent    4T
*/

#ifndef _LIB_RUNTIME_KEYBOARD_REDEFINE_H_
#define _LIB_RUNTIME_KEYBOARD_REDEFINE_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#ifndef COMPILER_VISUAL_STUDIO

#define VK_ESCAPE           0x1B
#define VK_TAB              0x09
#define VK_SHIFT            0x10
#define VK_CONTROL          0x11
#define VK_BACK             0x08
#define VK_RETURN           0x0D
#define VK_INSERT           0x2D
#define VK_HOME             0x24
#define VK_PRIOR            0x21
#define VK_DELETE           0x2E
#define VK_END              0x23
#define VK_NEXT             0x22
#define VK_UP               0x26
#define VK_LEFT             0x25
#define VK_DOWN             0x28
#define VK_RIGHT            0x27
#define VK_DIVIDE           0x6F
#define VK_MULTIPLY         0x6A
#define VK_SUBTRACT         0x6C
#define VK_ADD              0x6B
#define VK_CLEAR            0x0C
#define VK_SPACE            0x20
#define VK_DECIMAL          0x6E
#define VK_F1               0x70
#define VK_F2               0x71
#define VK_F3               0x72
#define VK_F4               0x73
#define VK_F5               0x74
#define VK_F6               0x75
#define VK_F7               0x76
#define VK_F8               0x77
#define VK_F9               0x78
#define VK_F10              0x79
#define VK_F11              0x7A
#define VK_F12              0x7B
#define VK_NUMPAD0          0x60
#define VK_NUMPAD1          0x61
#define VK_NUMPAD2          0x62
#define VK_NUMPAD3          0x63
#define VK_NUMPAD4          0x64
#define VK_NUMPAD5          0x65
#define VK_NUMPAD6          0x66
#define VK_NUMPAD7          0x67
#define VK_NUMPAD8          0x68
#define VK_NUMPAD9          0x69

#endif // not COMPILER_VISUAL_STUDIO

#endif // _LIB_RUNTIME_KEYBOARD_REDEFINE_H_
