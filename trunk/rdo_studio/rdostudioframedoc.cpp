#include "stdafx.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudiomodel.h"
#include "rdostudioframemanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioFrameDoc, CDocument)
	//{{AFX_MSG_MAP(RDOStudioFrameDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioFrameDoc::RDOStudioFrameDoc(): CDocument()
{
}

RDOStudioFrameDoc::~RDOStudioFrameDoc()
{
}

BOOL RDOStudioFrameDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioFrameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

#ifdef _DEBUG
void RDOStudioFrameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RDOStudioFrameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif

RDOStudioFrameView* RDOStudioFrameDoc::getView() const
{
	POSITION pos = GetFirstViewPosition();
	if ( pos ) {
		return static_cast<RDOStudioFrameView*>(GetNextView( pos ));
	}
	return NULL;
}

void RDOStudioFrameDoc::OnCloseDocument()
{
	int index = model->frameManager.findFrameIndex( this );

	CSyncObject* ppObjects[] = { model->frameManager.getFrameUsed( index ), model->frameManager.getFrameDraw( index ) };
	CMultiLock locks( ppObjects, 2 );
	locks.Lock();

//	CSingleLock lock_used( model->frameManager.getFrameUsed( index ) );
//	lock_used.Lock();

//	CSingleLock lock_draw( model->frameManager.getFrameDraw( index ) );
//	lock_draw.Lock();

	if ( !model->frameManager.isDeleted( index ) ) {
		model->frameManager.setDeleted( index, true );
	}
	model->frameManager.disconnectFrameDoc( this );

	CDocument::OnCloseDocument();

	locks.Unlock();
//	lock_draw.Unlock();
//	lock_used.Unlock();
}
