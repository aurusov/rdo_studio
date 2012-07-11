/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/abstract/headers/memdc/memdc.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2012
  \brief     Абстрактный DeviceContext, созданный в памяти
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		template <class TDC, class TBMP>
		inline AbstractMemDC<TDC, TBMP>::AbstractMemDC()
		   : m_width (0)
		   , m_height(0)
		{}

		template <class TDC, class TBMP>
		inline AbstractMemDC<TDC, TBMP>::~AbstractMemDC()
		{
		   clear();
		}

		template <class TDC, class TBMP>
		inline rbool AbstractMemDC<TDC, TBMP>::valid() const
		{
		   return m_pDC.get() != NULL ? true : false;
		}

		template <class TDC, class TBMP>
		inline rbool AbstractMemDC<TDC, TBMP>::create(ruint width, ruint height)
		{
		   HWND hWnd = ::GetDesktopWindow(); if (!hWnd) return false;
		   HDC  hDC  = ::GetDC(hWnd);        if (!hDC ) return false;

		   TDC dc(hDC);
		   rbool ok = create(width, height, dc);
		   ::ReleaseDC(hWnd, hDC);
		   return ok;
		}

		template <class TDC, class TBMP>
		inline rbool AbstractMemDC<TDC, TBMP>::create(ruint width, ruint height, REF(TDC) from)
		{
		   if (m_pBitmap.get() || m_pDC.get())
			  return false;

		   m_width  = width;
		   m_height = height;
		   m_pBitmap.reset(new TBMP(m_width, m_height, &from));
		   if (m_pBitmap.get())
		   {
			  m_pDC.reset(new TDC(m_pBitmap.get()));
		   }
		   return valid();
		}

		template <class TDC, class TBMP>
		inline rbool AbstractMemDC<TDC, TBMP>::resize(ruint width, ruint height)
		{
		   if (width == 0 || height == 0)
			  return false;
		   if (width == m_width && height == m_height)
			  return true;

		   clear();
		   return create(width, height);
		}

		template <class TDC, class TBMP>
		inline ruint AbstractMemDC<TDC, TBMP>::width() const
		{
		   return m_width;
		}

		template <class TDC, class TBMP>
		inline ruint AbstractMemDC<TDC, TBMP>::height() const
		{
		   return m_height;
		}

		template <class TDC, class TBMP>
		inline REF(TDC) AbstractMemDC<TDC, TBMP>::dc()
		{
		   return *m_pDC.get();
		}

		template <class TDC, class TBMP>
		inline REF(TBMP) AbstractMemDC<TDC, TBMP>::buffer()
		{
		   return *m_pBitmap.get();
		}

		template <class TDC, class TBMP>
		inline void AbstractMemDC<TDC, TBMP>::clear()
		{
		   if (m_pBitmap.get())  m_pBitmap.reset();
		   if (m_pDC    .get())  m_pDC    .reset();
		}

	} // namespace gui
} // namespace rdo
