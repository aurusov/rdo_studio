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
#include <QtGui/qpainter.h>
#include <QtGui/qpixmap.h>
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
			if (m_pBitmap.get())
			{
				m_pDC.reset(new QPainter(m_pBitmap.get()));
			}
			return true;
		}

	} // namespace gui
} // namespace rdo

#endif // _UI_QT_MEMDC_H_
