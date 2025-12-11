#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <memory>
#include <QtGui/qpainter.h>
#include <QtGui/qpixmap.h>
#include <boost/config.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/abstract/headers/memdc/memdc.h"
// --------------------------------------------------------------------------------

namespace rdo
{
    namespace gui
    {
        typedef  MemDC<QPainter, QPixmap>  QtMemDC;

        template <>
        inline bool MemDC<QPainter, QPixmap>::onCreate()
        {
            m_pBitmap.reset(new QPixmap(m_width, m_height));
            m_pDC.reset(new QPainter);
            return true;
        }

        template <>
        inline bool MemDC<QPainter, QPixmap>::resize(std::size_t width, std::size_t height)
        {
            if (width == 0 || height == 0)
                return false;
            if (width == m_width && height == m_height)
                return true;

            typedef std::unique_ptr<QPixmap> QPixmapPointer;

            QPixmapPointer pPrevBitmap;
            std::swap(pPrevBitmap, m_pBitmap);

            clear();
            if (!create(width, height))
                return false;

            if (pPrevBitmap.get())
            {
                QPainter painter(m_pBitmap.get());
                painter.drawPixmap(0, 0, *pPrevBitmap.get());
            }

            return true;
        }

    } // namespace gui
} // namespace rdo
