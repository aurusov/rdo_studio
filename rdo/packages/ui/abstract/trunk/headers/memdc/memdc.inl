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
		inline MemDC<TDC, TBMP>::MemDC()
			: MemDCBase()
		{}

		template <class TDC, class TBMP>
		inline MemDC<TDC, TBMP>::~MemDC()
		{
			clear();
		}

		template <class TDC, class TBMP>
		inline rbool MemDC<TDC, TBMP>::valid() const
		{
			return m_pDC.get() != NULL ? true : false;
		}

		template <class TDC, class TBMP>
		inline rbool MemDC<TDC, TBMP>::create(ruint width, ruint height)
		{
			if (m_pBitmap.get() || m_pDC.get())
				return false;

			m_width  = width;
			m_height = height;

			if (!onCreate())
				return false;

			return valid();
		}

		template <class TDC, class TBMP>
		inline rbool MemDC<TDC, TBMP>::resize(ruint width, ruint height)
		{
			if (width == 0 || height == 0)
				return false;
			if (width == m_width && height == m_height)
				return true;

			clear();
			return create(width, height);
		}

		template <class TDC, class TBMP>
		inline REF(TDC) MemDC<TDC, TBMP>::dc()
		{
			return *m_pDC.get();
		}

		template <class TDC, class TBMP>
		inline REF(TBMP) MemDC<TDC, TBMP>::buffer()
		{
			return *m_pBitmap.get();
		}

		template <class TDC, class TBMP>
		inline void MemDC<TDC, TBMP>::clear()
		{
			if (m_pBitmap.get())  m_pBitmap.reset();
			if (m_pDC    .get())  m_pDC    .reset();
		}

	} // namespace gui
} // namespace rdo
