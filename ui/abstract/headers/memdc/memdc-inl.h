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
		inline bool MemDC<TDC, TBMP>::valid() const
		{
			return m_pDC.get() != NULL ? true : false;
		}

		template <class TDC, class TBMP>
		inline bool MemDC<TDC, TBMP>::create(std::size_t width, std::size_t height)
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
		inline bool MemDC<TDC, TBMP>::resize(std::size_t width, std::size_t height)
		{
			if (width == 0 || height == 0)
				return false;
			if (width == m_width && height == m_height)
				return true;

			clear();
			return create(width, height);
		}

		template <class TDC, class TBMP>
		inline TDC& MemDC<TDC, TBMP>::dc()
		{
			return *m_pDC.get();
		}

		template <class TDC, class TBMP>
		inline TBMP& MemDC<TDC, TBMP>::buffer()
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
