/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      memdc.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.03.2012
  \brief     DeviceContext, созданный в памяти
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "ui/mfc_ctrls/rdo_ctrls_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/memdc.h"
// --------------------------------------------------------------------------------

MemDC::MemDC()
   : m_width (0)
   , m_height(0)
{}

MemDC::~MemDC()
{
   clear();
}

rbool MemDC::valid() const
{
   return m_pDC.get() != NULL ? true : false;
}

rbool MemDC::create(ruint width, ruint height)
{
   HWND hWnd = ::GetDesktopWindow(); if (!hWnd) return false;
   HDC  hDC  = ::GetDC(hWnd);        if (!hDC ) return false;

   TDC dc(hDC);
   rbool ok = create(width, height, dc);
   ::ReleaseDC(hWnd, hDC);
   return ok;
}

rbool MemDC::create(ruint width, ruint height, REF(TDC) from)
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

rbool MemDC::resize(ruint width, ruint height)
{
   if (width == 0 || height == 0)
      return false;
   if (width == m_width && height == m_height)
      return true;

   clear();
   return create(width, height);
}

ruint MemDC::width() const
{
   return m_width;
}

ruint MemDC::height() const
{
   return m_height;
}

REF(MemDC::TDC) MemDC::dc()
{
   return *m_pDC.get();
}

REF(MemDC::TBMP) MemDC::buffer()
{
   return *m_pBitmap.get();
}

void MemDC::clear()
{
   if (m_pBitmap.get())  m_pBitmap.reset();
   if (m_pDC    .get())  m_pDC    .reset();
}
