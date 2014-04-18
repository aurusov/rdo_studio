/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/abstract/headers/memdc/memdc.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2012
  \brief     Абстрактный DeviceContext, созданный в памяти
  \indent    4T
*/

#ifndef _UI_ABSTRACT_MEMDC_H_
#define _UI_ABSTRACT_MEMDC_H_

// ----------------------------------------------------------------------- INCLUDES
#include <memory>
#include <boost/config.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		class MemDCBase
		{
		public:
			 MemDCBase();
			~MemDCBase();

			std::size_t width() const;
			std::size_t height() const;

		protected:
			std::size_t m_width;
			std::size_t m_height;
		};

		template <class TDC, class TBMP>
		class MemDC: public MemDCBase
		{
		public:
			 MemDC();
			~MemDC();

			bool valid() const;
			bool create(std::size_t width, std::size_t height);
			bool create(std::size_t width, std::size_t height, TDC& from);
			bool resize(std::size_t width, std::size_t height);

			TDC& dc();
			TBMP& buffer();

		private:
#ifdef BOOST_NO_CXX11_SMART_PTR
			std::auto_ptr<TDC> m_pDC;
			std::auto_ptr<TBMP> m_pBitmap;
#else
			std::unique_ptr<TDC> m_pDC;
			std::unique_ptr<TBMP> m_pBitmap;
#endif
			bool onCreate();

			void clear();
		};

	} // namespace gui
} // namespace rdo

#include "ui/abstract/headers/memdc/memdc.inl"

#endif // _UI_ABSTRACT_MEMDC_H_
