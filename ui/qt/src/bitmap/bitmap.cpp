// ---------------------------------------------------------------------------- PCH
#include "ui/qt/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/qt/headers/bitmap/bitmap.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui;

QPixmap Bitmap::transparent(const QPixmap& bitmap, const QPixmap& mask)
{
    QPixmap result;

    if (bitmap.isNull() || mask.isNull() || bitmap.size() != mask.size())
        return result;

    QImage generatedImage = bitmap.copy().toImage().convertToFormat(QImage::Format_ARGB32);

    result = QPixmap::fromImage(Bitmap::transparent(generatedImage, mask.toImage()));
    return result;
}

QImage Bitmap::transparent(const QImage& bitmap, const QImage& mask)
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
