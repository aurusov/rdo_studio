/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      bitmap.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.07.2012
  \brief     Дополнительные функции работы с картинками
  \indent    4T
*/

#ifndef _UI_QT_BITMAP_H_
#define _UI_QT_BITMAP_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <map>
#include <QPixmap>
#include <QString>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		class Bitmap
		{
		public:
			static QPixmap transparent(CREF(QPixmap) bitmap, CREF(QPixmap) mask);
			static QImage  transparent(CREF(QImage)  bitmap, CREF(QImage)  mask);
		};

		typedef  std::map<QString, QPixmap>  BitmapList;

	} // namespace gui
} // namespace rdo

#endif // _UI_GDIPLUS_BITMAP_H_
