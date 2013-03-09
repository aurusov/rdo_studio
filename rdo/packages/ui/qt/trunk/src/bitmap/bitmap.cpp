/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      bitmap.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.07.2012
  \brief     Дополнительные функции работы с картинками
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "ui/qt/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/qt/headers/bitmap/bitmap.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui;

QPixmap Bitmap::transparent(CREF(QPixmap) bitmap, CREF(QPixmap) mask)
{
	QPixmap result;

	if (bitmap.isNull() || mask.isNull() || bitmap.size() != mask.size())
		return result;

	QImage generatedImage = bitmap.copy().toImage().convertToFormat(QImage::Format_ARGB32);

	result = QPixmap::fromImage(Bitmap::transparent(generatedImage, mask.toImage()));
	return result;
}

QImage Bitmap::transparent(CREF(QImage) bitmap, CREF(QImage) mask)
{
	QImage result(bitmap);

	if (bitmap.isNull() || mask.isNull() || bitmap.size() != mask.size())
		return result;

	QRgb white       = QColor(Qt::white).rgb();
	QRgb transparent = QColor(Qt::transparent).rgba();
	for (int x = 0; x < mask.width(); ++x)
	{
		for (int y = 0; y < mask.height(); ++y)
		{
			if (mask.pixel(x, y) == white)
			{
				result.setPixel(x, y, transparent);
			}
		}
	}

	return result;
}
