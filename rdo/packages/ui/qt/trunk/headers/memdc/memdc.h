/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/qt/headers/memdc/memdc.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.07.2012
  \brief     DeviceContext, созданный в памяти
  \indent    4T
*/

#ifndef _UI_QT_MEMDC_H_
#define _UI_QT_MEMDC_H_

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
		inline rbool MemDC<QPainter, QPixmap>::onCreate()
		{
			m_pBitmap.reset(new QPixmap(m_width, m_height));
			m_pDC.reset(new QPainter);
			return true;
		}

		template <>
		inline rbool MemDC<QPainter, QPixmap>::resize(ruint width, ruint height)
		{
			if (width == 0 || height == 0)
				return false;
			if (width == m_width && height == m_height)
				return true;

#ifdef BOOST_NO_CXX11_SMART_PTR
			typedef std::auto_ptr<QPixmap> QPixmapPointer;
#else
			typedef std::unique_ptr<QPixmap> QPixmapPointer;
#endif
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

#endif // _UI_QT_MEMDC_H_
