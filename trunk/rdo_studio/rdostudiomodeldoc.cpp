#include "stdafx.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudiomodel.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioModelDoc, RDOStudioEditBaseDoc)

BEGIN_MESSAGE_MAP(RDOStudioModelDoc, RDOStudioEditBaseDoc)
	//{{AFX_MSG_MAP(RDOStudioModelDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioModelDoc::RDOStudioModelDoc(): RDOStudioEditBaseDoc()
{
}

RDOStudioModelDoc::~RDOStudioModelDoc()
{
}

BOOL RDOStudioModelDoc::OnNewDocument()
{
	if ( !RDOStudioEditBaseDoc::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioModelDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

#ifdef _DEBUG
void RDOStudioModelDoc::AssertValid() const
{
	RDOStudioEditBaseDoc::AssertValid();
}

void RDOStudioModelDoc::Dump(CDumpContext& dc) const
{
	RDOStudioEditBaseDoc::Dump(dc);
}
#endif

void RDOStudioModelDoc::updateModify()
{
	RDOStudioModelView* view = getView();
	if ( view ) {
		for ( int i = 0; i < view->tab->getItemCount(); i++ ) {
			if ( view->tab->getItemEdit( i )->isModify() ) {
				SetModifiedFlag( true );
				return;
			}
		}
	}
	SetModifiedFlag( false );
}

RDOStudioModelView* RDOStudioModelDoc::getView() const
{
	POSITION pos = GetFirstViewPosition();
	if ( pos ) {
		return static_cast<RDOStudioModelView*>(GetNextView( pos ));
	}
	return NULL;
}

BOOL RDOStudioModelDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if ( model->getModelDoc() == this ) {
		return model->canCloseDocument();
	}
	return TRUE;
}

void RDOStudioModelDoc::OnCloseDocument() 
{
	RDOStudioEditBaseDoc::OnCloseDocument();
}

bool RDOStudioModelDoc::isRunning() const
{
	return model->isRunning();
}
