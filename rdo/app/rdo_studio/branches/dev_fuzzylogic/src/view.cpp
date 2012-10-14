/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/view.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioView
// --------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOStudioView, CView)

BEGIN_MESSAGE_MAP(RDOStudioView, CView)
END_MESSAGE_MAP()

RDOStudioView::RDOStudioView(): CView()
{
}

RDOStudioView::~RDOStudioView()
{
}

#ifdef _DEBUG
void RDOStudioView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
