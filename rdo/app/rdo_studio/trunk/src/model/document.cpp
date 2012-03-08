#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/src/model/document.h"
#include "app/rdo_studio_mfc/src/model/view.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelDoc
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioModelDoc, RDOStudioEditBaseDoc)

BEGIN_MESSAGE_MAP(RDOStudioModelDoc, RDOStudioEditBaseDoc)
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
		AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODEL_NEED_STOPED_FOR_CLOSE ).c_str(), NULL, MB_ICONWARNING | MB_OK );
		return false;
	}
	bool flag = true;
	if ( isModify() ) {
		switch ( AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL ) ) {
			case IDYES   : flag = model->saveModel(); break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	} else if ( !RDOStudioMainFrame::is_close_mode() ) {
		switch ( AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODELCLOSE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNO ) ) {
			case IDYES: flag = true; break;
			case IDNO : flag = false; break;
		}
	}
	if ( flag ) {
		model->m_autoDeleteDoc = false;
		model->closeModel();
		model->m_autoDeleteDoc = true;
		model->m_GUI_HAS_MODEL = false;
	}
	return flag;
}

void RDOStudioModelDoc::setName( const std::string& str )
{
	name = str;
	rdo::trim( name );
	if ( studioApp.getShowCaptionFullName() ) {
		SetTitle( rdo::format( IDS_MODEL_NAME, model->getFullName().c_str() ).c_str()  );
	} else {
		SetTitle( rdo::format( IDS_MODEL_NAME, name.c_str() ).c_str() );
	}
}

bool RDOStudioModelDoc::isModify()
{
	updateModify();
	return IsModified() ? true : false;
}
