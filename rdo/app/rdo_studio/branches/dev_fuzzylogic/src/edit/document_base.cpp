/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/document_base.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/document_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseDoc
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioEditBaseDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioEditBaseDoc, CDocument)
END_MESSAGE_MAP()

RDOStudioEditBaseDoc::RDOStudioEditBaseDoc(): CDocument()
{
}

RDOStudioEditBaseDoc::~RDOStudioEditBaseDoc()
{
}

BOOL RDOStudioEditBaseDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioEditBaseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

#ifdef _DEBUG
void RDOStudioEditBaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RDOStudioEditBaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
