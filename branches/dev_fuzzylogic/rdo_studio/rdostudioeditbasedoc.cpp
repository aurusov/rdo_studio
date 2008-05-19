#include "stdafx.h"
#include "rdostudioeditbasedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioEditBaseDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioEditBaseDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioEditBaseDoc, CDocument)
	//{{AFX_MSG_MAP(RDOStudioEditBaseDoc)
	//}}AFX_MSG_MAP
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
