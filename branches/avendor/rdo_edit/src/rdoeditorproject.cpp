#include "stdafx.h"
#include "rdoeditorproject.h"
#include "rdoeditortabctrl.h"
#include "rdoeditormainfrm.h"
#include "rdoeditorapp.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorProject
// ----------------------------------------------------------------------------
RDOEditorProject rdoEditorProject;

RDOEditorProject::RDOEditorProject():
	name( "" ),
	lastModelPath( "" ),
	modelFileName( "" ),
	running( false ),
	buf1( "" ),
	buf2( "" ),
	buf3( "" ),
	buf4( "" ),
	resetBuf1( true ),
	resetBuf2( true ),
	resetBuf3( true ),
	resetBuf4( true ),
	currentBuffer( 1 ),
	canClearBuffer( true ),
	clearBufferDelay( 7 ),
	rdoFileName( "" )
{
}

RDOEditorProject::~RDOEditorProject()
{
	rdoEditorApp.WriteProfileInt( "buffer", "clear", canClearBuffer );
	rdoEditorApp.WriteProfileInt( "buffer", "delay", clearBufferDelay );
}

void RDOEditorProject::newProject()
{
	if ( closeProject() ) {

		RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;

		for ( int i = 0; i < tabCtrl->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tabCtrl->getItemEdit( i );
			edit->setReadOnly( false );
			edit->clearAll();
			int resID = -1;
			switch ( i ) {
				case RDOEDIT_PAT: resID = ID_INSERT_PAT_PATOPERATION; break;
				case RDOEDIT_RTP: resID = ID_INSERT_RTP_RTPPERMANENT; break;
				case RDOEDIT_RSS: resID = ID_INSERT_RSS_RSS; break;
				case RDOEDIT_OPR: resID = ID_INSERT_OPR_OPR; break;
				case RDOEDIT_FRM: resID = ID_INSERT_FRM_FRM; break;
				case RDOEDIT_FUN: resID = ID_INSERT_FUN_FUN; break;
				case RDOEDIT_SMR: resID = ID_INSERT_SMR_SMR; break;
			}

			CString s = "";

			if ( resID != - 1 && s.LoadString( resID ) ) {

				int incPos = -1;
				switch ( resID ) {
					case ID_INSERT_PAT_PATOPERATION     : incPos = 9; break;
					case ID_INSERT_RTP_RTPPERMANENT     : incPos = 15; break;
					case ID_INSERT_RSS_RSS              : incPos = 13; break;
					case ID_INSERT_OPR_OPR              : incPos = 14; break;
					case ID_INSERT_FRM_FRM              : incPos = 7; break;
					case ID_INSERT_FUN_FUN              : incPos = 10; break;
					case ID_INSERT_SMR_SMR              : incPos = 17; break;
				}

				edit->replaceCurrent( s, incPos );
			}

			edit->setModifyFalse();
		}

		setName( "" );
	}
}

CString RDOEditorProject::loadProject( const CString& projectName )
{
	if ( closeProject() ) {

		bool flag = true;
		bool readOnly = false;
		if ( projectName.IsEmpty() ) {
			CString s;
			s.LoadString( ID_MODEL_FILETYPE );
			CFileDialog dlg( true, "smr", lastModelPath, 0, s );
			flag = dlg.DoModal() == IDOK;
			readOnly = dlg.GetReadOnlyPref() == TRUE;
			if ( flag ) {
				extractName( dlg );
				changeLastModelPath( dlg.GetPathName() );
			}
		} else {
			extractName( projectName );
			flag = !name.IsEmpty();
			changeLastModelPath( projectName );
		}

		if ( flag ) {

			CString fName;
			RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;

			bool wasError = false;

			for ( int i = 0; i < tabCtrl->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tabCtrl->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();

				fName = modelFileName;
				bool canLoad  = true;
				bool mustExis = false;
				switch ( i ) {
					case RDOEDIT_PAT: fName += ".pat"; break;
					case RDOEDIT_RTP: fName += ".rtp"; break;
					case RDOEDIT_RSS: fName += ".rss"; break;
					case RDOEDIT_OPR: fName += ".opr"; break;
					case RDOEDIT_FRM: fName += ".frm"; break;
					case RDOEDIT_FUN: fName += ".fun"; break;
					case RDOEDIT_SMR: fName += ".smr"; mustExis = true; break;
					case RDOEDIT_DPT: fName += ".dpt"; break;
					case RDOEDIT_PMD: fName += ".pmd"; break;
					default: canLoad = false;
				}

				if ( canLoad ) {
					wasError |= !loadFile( fName, edit, mustExis );
					edit->setReadOnly( readOnly );
				}

				edit->setCurrentPos( 0 );
				edit->setModifyFalse();
			}
			if ( !wasError ) {
				return modelFileName;
			} else {
				CString s;
				s.LoadString( ID_MSG_MODELOPEN_ERROR );
				AfxGetMainWnd()->MessageBox( s, NULL, MB_ICONSTOP | MB_OK );
				setName( "" );
			}
		}
	}
	return "";
}

bool RDOEditorProject::loadFile( const CString& fName, RDOEditorEdit* edit, const bool mustExist ) const
{
	bool res = true;
	CFile f;
	TRY {
		CFileStatus status;
		if( CFile::GetStatus( fName, status ) ) {
			f.Open( fName, CFile::modeRead );
			edit->load( f );
			f.Close();
		} else {
			if ( mustExist ) res = false;
		}
	} CATCH( CFileException, e ) {
		res = false;
	}
	END_CATCH

	return res;
}

void RDOEditorProject::loadPMV() const
{
	if ( canUpdateResults() ) {

		RDOEditorEdit* edit = ((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab.getItemEdit( RDOEDIT_PMV );
		edit->clearAll();

		if ( !loadFile( modelFileName + ".pmv", edit ) ) {
			CString s;
			s.LoadString( ID_MSG_MODELOPEN_PMV_ERROR );
			AfxGetMainWnd()->MessageBox( s, NULL, MB_ICONSTOP | MB_OK );
		}

		edit->setCurrentPos( 0 );
		edit->setModifyFalse();
	}
}

void RDOEditorProject::loadTRC() const
{
	if ( canUpdateResults() ) {

		RDOEditorEdit* edit = ((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab.getItemEdit( RDOEDIT_TRC );
		edit->clearAll();

		if ( !loadFile( modelFileName + ".trc", edit ) ) {
			CString s;
			s.LoadString( ID_MSG_MODELOPEN_TRC_ERROR );
			AfxGetMainWnd()->MessageBox( s, NULL, MB_ICONSTOP | MB_OK );
		}

		edit->setCurrentPos( 0 );
		edit->setModifyFalse();
	}
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

void RDOEditorProject::extractName( const CFileDialog& dlg )
{
	modelFileName = dlg.GetPathName();
	CString s1    = dlg.GetFileName();
	CString s2    = dlg.GetFileExt();
	if ( !s2.IsEmpty() ) {
		setName( s1.Left( s1.GetLength() - s2.GetLength() - 1 ) );
		modelFileName = modelFileName.Left( modelFileName.GetLength() - s2.GetLength() - 1 );
	} else {
		setName( s1 );
	}
}

void RDOEditorProject::extractName( CString str )
{
	int pos = str.ReverseFind( '.' );
	if ( pos != -1 ) {
		str = str.Left( pos );
	}
	str.TrimLeft();
	str.TrimRight();
	modelFileName = str;
	pos = str.ReverseFind( '\\' );
	if ( pos == -1 ) {
		pos = str.ReverseFind( '/' );
	}
	if ( pos != -1 ) {
		setName( str.Right( str.GetLength() - pos - 1 ) );
	} else {
		setName( "" );
	}
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

bool RDOEditorProject::isModify() const
{
	RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
	bool flag = false;
	for ( int i = 0; i < tabCtrl->getItemCount(); i++ ) {
		flag = tabCtrl->getItemEdit( i )->isModify();
		if ( flag ) break;
	}
	return flag;
}

void RDOEditorProject::setName( const CString str )
{
	name = str;
	name.TrimLeft();
	name.TrimRight();
	if ( name.IsEmpty() ) {
		modelFileName = "";
	}
	rdoEditorApp.setCaption( name );
}

void RDOEditorProject::start()
{
	running = true;
}

void RDOEditorProject::stop()
{
	running = false;
}

void RDOEditorProject::changeLastModelPath( CString str )
{
	if ( str.GetLength() ) {
		lastModelPath = rdoEditorApp.extractFilePath( str ) + "*.smr";
	} else {
		lastModelPath = "";
	}
	rdoEditorApp.WriteProfileString( "reopen", "lastModelPath", lastModelPath );
}
