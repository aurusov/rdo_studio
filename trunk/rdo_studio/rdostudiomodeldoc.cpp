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

using namespace std;

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
	name( "" ),
	running( false )
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

BOOL RDOStudioModelDoc::CanCloseFrame(CFrameWnd* /*pFrame*/) 
{
	bool flag = true;
	if ( isModify() ) {
		int res = AfxGetMainWnd()->MessageBox( format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = model->saveModel(); break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	}
	if ( flag ) {
		model->closeWithDocDelete = false;
		model->closeModel();
		model->closeWithDocDelete = true;
	}
	return flag;
}

void RDOStudioModelDoc::OnCloseDocument() 
{
	model->stopModel();
	RDOStudioEditBaseDoc::OnCloseDocument();
}

string RDOStudioModelDoc::getName() const
{
	return name;
}

void RDOStudioModelDoc::setName( const string& str )
{
	name = str;
	trim( name );
	SetTitle( format( IDS_MODEL_NAME, name.c_str() ).c_str()  );
}

bool RDOStudioModelDoc::isModify()
{
	updateModify();
	return IsModified() ? true : false;
}
