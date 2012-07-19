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
	if (bitmap.isNull() || mask.isNull() || bitmap.size() != mask.size())
		return QPixmap();

	QImage generatedImage = bitmap.copy().toImage().convertToFormat(QImage::Format_ARGB32);

	Bitmap::transparent(generatedImage, mask.toImage());

	if (generatedImage.isNull())
		return QPixmap();

	QPixmap generated = QPixmap::fromImage(generatedImage);
	if (generated.isNull())
		return QPixmap();

	return generated;
}

rbool Bitmap::transparent(REF(QImage) inOutBitmap, CREF(QImage) mask)
{
	if (inOutBitmap.isNull() || mask.isNull() || inOutBitmap.size() != mask.size())
		return false;

	QRgb white       = QColor(Qt::white).rgb();
	QRgb transparent = QColor(Qt::transparent).rgba();
	for (int x = 0; x < mask.width(); ++x)
	{
		for (int y = 0; y < mask.height(); ++y)
		{
			if (mask.pixel(x, y) == white)
			{
				inOutBitmap.setPixel(x, y, transparent);
			}
		}
	}

	return true;
}
