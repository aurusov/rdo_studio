/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      memdc.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.03.2012
  \brief     DeviceContext, созданный в памяти
  \indent    4T
*/

#ifndef _UI_MFC_CTRLS_MEMDC_H_
#define _UI_MFC_CTRLS_MEMDC_H_

// ----------------------------------------------------------------------- INCLUDES
#include <memory>
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

class MemDC
{
public:
	typedef  Gdiplus::Graphics  TDC;
	typedef  Gdiplus::Bitmap    TBMP;

    MemDC();
   ~MemDC();

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

#endif // _UI_MFC_CTRLS_MEMDC_H_
