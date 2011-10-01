/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime.cpp
  \authors   Евгений Пройдаков (lord.tiran@gmail.com)
  \date      16.05.2007
  \brief     keyboard
  \indent    4T
*/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// --------------------------------------------------------------------------------

#ifndef OST_WINDOWS

#define VK_ESCAPE           1
#define VK_TAB              15
#define VK_SHIFT            301 // TODO find code
#define VK_CONTROL          302 // TODO find code
#define VK_BACK             14
#define VK_RETURN           303 // TODO find code
#define VK_INSERT           304 // TODO find code
#define VK_HOME             102
#define VK_PRIOR            305 // TODO find code
#define VK_DELETE           111
#define VK_END              107
#define VK_NEXT             306 // TODO find code
#define VK_UP               103
#define VK_LEFT             105
#define VK_DOWN             108
#define VK_RIGHT            106
#define VK_DIVIDE           98
#define VK_MULTIPLY         55         
#define VK_SUBTRACT         74         
#define VK_ADD              78
#define VK_CLEAR            307 // TODO find code
#define VK_SPACE            57
#define VK_DECIMAL          83 
#define VK_F1               59
#define VK_F2               60
#define VK_F3               61
#define VK_F4               62
#define VK_F5               63
#define VK_F6               64
#define VK_F7               65
#define VK_F8               66
#define VK_F9               67
#define VK_F10              68
#define VK_F11              87
#define VK_F12              88
#define VK_NUMPAD0          308 // TODO find code          
#define VK_NUMPAD1          309 // TODO find code
#define VK_NUMPAD2          310 // TODO find code
#define VK_NUMPAD3          311 // TODO find code
#define VK_NUMPAD4          312 // TODO find code
#define VK_NUMPAD5          313 // TODO find code
#define VK_NUMPAD6          314 // TODO find code
#define VK_NUMPAD7          315 // TODO find code
#define VK_NUMPAD8          316 // TODO find code
#define VK_NUMPAD9          317 // TODO find code

#endif // OST_WINDOWS

#endif //_KEYBOARD_H_
