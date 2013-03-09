/*!
  \copyright (c) RDO-Team, 2013
  \file      warning_disable.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.03.2013
  \brief     
  \indent    4T
*/

#include "utils/platform.h"

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(push)
	#pragma warning(disable: 4100)
	#pragma warning(disable: 4127)
	#pragma warning(disable: 4231)
	#pragma warning(disable: 4244)
	#pragma warning(disable: 4251)
	#pragma warning(disable: 4512)
	#pragma warning(disable: 4800)
	#pragma warning(disable: 4913)
#endif
