#include "stdafx.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>

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

RDOStudioModelDoc::RDOStudioModelDoc():
	RDOStudioEditBaseDoc(),
	name( "" )
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

BOOL RDOStudioModelDoc::SaveModified()
{
	if ( model->isRunning() ) {
		AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODEL_NEED_STOPED ).c_str(), NULL, MB_ICONWARNING | MB_OK );
		return false;
	}
	bool flag = true;
	if ( isModify() ) {
		int res = AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = model->saveModel(); break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	}
	if ( flag ) {
		model->autoDeleteDoc = false;
		model->closeModel();
		model->autoDeleteDoc = true;
	}
	return flag;
}

void RDOStudioModelDoc::setName( const std::string& str )
{
	name = str;
	rdo::trim( name );
	if ( studioApp.getShowCaptionFullName() ) {
		SetTitle( rdo::format( IDS_MODEL_NAME, kernel->repository()->getFullName().c_str() ).c_str()  );
	} else {
		SetTitle( rdo::format( IDS_MODEL_NAME, name.c_str() ).c_str() );
	}
}

bool RDOStudioModelDoc::isModify()
{
	updateModify();
	return IsModified() ? true : false;
}
