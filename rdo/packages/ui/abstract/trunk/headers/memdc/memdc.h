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
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		template <class TDC, class TBMP>
		class AbstractMemDC
		{
		public:
			typedef  TDC   device_type;
			typedef  TBMP  buffer_type;

			 AbstractMemDC();
			~AbstractMemDC();

			rbool valid () const;
			rbool create(ruint width, ruint height);
			rbool create(ruint width, ruint height, REF(TDC) from);
			rbool resize(ruint width, ruint height);

			ruint     width () const;
			ruint     height() const;
			REF(TDC)  dc    ();
			REF(TBMP) buffer();

		private:
			std::auto_ptr<TDC>    m_pDC;
			std::auto_ptr<TBMP>   m_pBitmap;
			ruint                 m_width;
			ruint                 m_height;

			void clear();
		};

	} // namespace gui
} // namespace rdo

#include "ui/abstract/headers/memdc/memdc.inl"

#endif // _UI_ABSTRACT_MEMDC_H_
