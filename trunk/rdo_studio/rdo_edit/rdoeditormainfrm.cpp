#include "stdafx.h"
#include "rdoeditormainfrm.h"

/*
CString RDOEditorMainFrame::loadRdoPath() const
{
	TRY {
		CString s = rdoEditorApp.GetProfileString( "RAO-simulator", "path", "" );

		CFileStatus status;
		if ( CFile::GetStatus( s, status ) ) {
			rdoEditorProject.rdoFileName = s;
		} else {
			rdoEditorProject.rdoFileName = "";
		}

	} CATCH( CException, e ) {
		rdoEditorProject.rdoFileName = "";
	}
	END_CATCH
	return rdoEditorProject.rdoFileName;
}

void RDOEditorMainFrame::saveRdoPath( const CString& path ) const
{
	TRY {
		rdoEditorApp.WriteProfileString( "RAO-simulator", "path", path );
	} CATCH( CException, e ) {
	}
	END_CATCH
}

void RDOEditorMainFrame::OnOptions()
{
	RDOEditorOptions dlg( childView.tab.getCurrentEdit()->getEditorStyle() );
	dlg.DoModal();
}

void RDOEditorMainFrame::OnHelpContents()
{
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return;

//	HtmlHelp( ::GetDesktopWindow(), filename, HH_DISPLAY_TOPIC, NULL );
}
*/
