#include "stdafx.h"
#include "rdoeditorproject.h"
#include "rdoeditortabctrl.h"
#include "rdoeditormainfrm.h"
#include "rdoeditorapp.h"
#include "../rdo_studio/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/*
// ----------------------------------------------------------------------------
// ---------- RDOEditorProject
// ----------------------------------------------------------------------------
//RDOEditorProject rdoEditorProject;

RDOEditorProject::RDOEditorProject():
	name( "" ),
	lastModelPath( "" ),
	modelFileName( "" ),
	running( false ),
	rdoFileName( "" )
{
}

bool RDOEditorProject::saveProject()
{
	if ( isModify() ) {

		if ( name.IsEmpty() ) {
			if ( !saveAsDlg() ) return false;
		}

	} else return true;

	save();

	return true;
}

void RDOEditorProject::saveAsProject()
{
	if ( saveAsDlg() ) {
		save();
	}
}

void RDOEditorProject::save() const
{
	CFile f;
	CString fName;

	RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;

	for ( int i = 0; i < tabCtrl->getItemCount(); i++ ) {
		fName = modelFileName;
		switch ( i ) {
			case RDOEDIT_PAT: fName += ".pat"; break;
			case RDOEDIT_RTP: fName += ".rtp"; break;
			case RDOEDIT_RSS: fName += ".rss"; break;
			case RDOEDIT_OPR: fName += ".opr"; break;
			case RDOEDIT_FRM: fName += ".frm"; break;
			case RDOEDIT_FUN: fName += ".fun"; break;
			case RDOEDIT_SMR: fName += ".smr"; break;
			case RDOEDIT_DPT: fName += ".dpt"; break;
			case RDOEDIT_PMD: fName += ".pmd"; break;
			case RDOEDIT_PMV: fName += ".pmv"; break;
			case RDOEDIT_TRC: fName += ".trc"; break;
		}

		RDOEditorEdit* edit = tabCtrl->getItemEdit( i );

		TRY {
			if ( edit->getLength() ) {
				f.Open( fName, CFile::modeCreate | CFile::modeWrite );
				edit->save( f );
				f.Close();
			} else {
				CFileStatus status;
				if( CFile::GetStatus( fName, status ) ) CFile::Remove( fName );
			}
		} CATCH( CFileException, e ) {
			CString s;
			s.LoadString( ID_MSG_MODELSAVE_ERROR );
			AfxGetMainWnd()->MessageBox( s, NULL, MB_ICONSTOP | MB_OK );
		}
		END_CATCH

		edit->setModifyFalse();
	}
}

bool RDOEditorProject::saveAsDlg()
{
	CString s;
	s.LoadString( ID_MODEL_FILETYPE );
	CFileDialog dlg( false, "smr", lastModelPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, s );

	if ( dlg.DoModal() == IDOK ) {

		changeLastModelPath( dlg.GetPathName() );

		extractName( dlg );

		if ( name.IsEmpty() ) return false;

		return true;

	}
	return false;
}

bool RDOEditorProject::closeProject()
{
	if ( isModify() ) {

		CString s;
		s.LoadString( ID_MSG_MODELSAVE_QUERY );
		int res = AfxGetMainWnd()->MessageBox( s, NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES: {
				if ( saveProject() ) {

					name          = "";
					modelFileName = "";

					RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
					for ( int i = 0; i < tabCtrl->getItemCount(); i++ ) {
						RDOEditorEdit* edit = tabCtrl->getItemEdit( i );
						edit->clearAll();
						edit->setModifyFalse();
					}

					return true;
				};
				return false;
			}
			case IDNO    : return true;
			case IDCANCEL: return false;
		}
	}
	return true;
}

void RDOEditorProject::start()
{
	running = true;
}

void RDOEditorProject::stop()
{
	running = false;
}

*/
