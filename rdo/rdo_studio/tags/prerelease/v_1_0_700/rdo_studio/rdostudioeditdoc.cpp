#include "stdafx.h"
#include "rdostudioeditdoc.h"
#include "rdostudioeditview.h"
#include "rdo_edit/rdoeditoredit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioEditDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioEditDoc, RDOStudioEditBaseDoc)

BEGIN_MESSAGE_MAP(RDOStudioEditDoc, RDOStudioEditBaseDoc)
	//{{AFX_MSG_MAP(RDOStudioEditDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioEditDoc::RDOStudioEditDoc(): RDOStudioEditBaseDoc()
{
}

RDOStudioEditDoc::~RDOStudioEditDoc()
{
}

BOOL RDOStudioEditDoc::OnNewDocument()
{
	if ( !RDOStudioEditBaseDoc::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

#ifdef _DEBUG
void RDOStudioEditDoc::AssertValid() const
{
	RDOStudioEditBaseDoc::AssertValid();
}

void RDOStudioEditDoc::Dump(CDumpContext& dc) const
{
	RDOStudioEditBaseDoc::Dump(dc);
}
#endif

void RDOStudioEditDoc::updateModify()
{
	POSITION pos = GetFirstViewPosition();
	if ( pos ) {
		RDOStudioEditView* view = static_cast<RDOStudioEditView*>(GetNextView( pos ));
		if ( view ) {
			if ( view->edit->isModify() ) {
				SetModifiedFlag( true );
				return;
			}
		}
	}
	SetModifiedFlag( false );
}
