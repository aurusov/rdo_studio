#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <map>
#include <QPixmap>
#include <QString>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		class Bitmap
		{
		public:
			static QPixmap transparent(const QPixmap& bitmap, const QPixmap& mask);
			static QImage  transparent(const QImage&  bitmap, const QImage&  mask);
		};

		typedef  std::map<QString, QPixmap>  BitmapList;

	} // namespace gui
} // namespace rdo
