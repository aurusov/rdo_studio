#include "stdafx.h"
#include "rdocatalogdoc.h"
#include "rdocatalogview.h"
#include "rdocatalogapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOCatalogDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOCatalogDoc, CDocument)
	//{{AFX_MSG_MAP(RDOCatalogDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOCatalogDoc::RDOCatalogDoc(): CDocument()
{
}

RDOCatalogDoc::~RDOCatalogDoc()
{
}

BOOL RDOCatalogDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOCatalogDoc::Serialize( CArchive& ar )
{
	if ( ar.IsStoring() ) {
	} else {
	}
}

#ifdef _DEBUG
void RDOCatalogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RDOCatalogDoc::Dump( CDumpContext& dc ) const
{
	CDocument::Dump( dc );
}
#endif

BOOL RDOCatalogDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	if ( !CDocument::OnOpenDocument(lpszPathName) ) return FALSE;

	POSITION pos = GetFirstViewPosition();
	static_cast<RDOCatalogView*>(GetNextView( pos ))->Navigate2( lpszPathName, NULL, NULL );

	return TRUE;
}
