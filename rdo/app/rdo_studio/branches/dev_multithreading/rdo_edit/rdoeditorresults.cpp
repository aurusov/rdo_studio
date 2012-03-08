/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresults.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResults
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP( RDOEditorResults, RDOEditorBaseEdit )
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOEditorResults::RDOEditorResults(): RDOEditorBaseEdit()
{
	kw0 = "TRUE FALSE";
	kw1 = "";
	kw2 = "";
	kw3 = "";
}

RDOEditorResults::~RDOEditorResults()
{
}

/*
BOOL RDOEditorResults::DestroyWindow()
{
	return RDOEditorBaseEdit::DestroyWindow();
}
*/

int RDOEditorResults::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOEditorBaseEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	setReadOnly( true );

	return 0;
}

void RDOEditorResults::setEditorStyle( RDOEditorResultsStyle* _style )
{
	RDOEditorBaseEdit::setEditorStyle( _style );
	if ( !style ) return;
}

void RDOEditorResults::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if ( s.find_first_of( keyword ) == tstring::npos || keyword.empty() ) {
		keyword = "pmv";
	}

	HH_AKLINK link;
	::ZeroMemory( &link, sizeof( HH_AKLINK ) );
	link.cbStruct     = sizeof( HH_AKLINK );
	link.fIndexOnFail = TRUE;
	link.pszKeywords  = keyword.c_str();

	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_KEYWORD_LOOKUP, (DWORD)&link );
}

void RDOEditorResults::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOEditorResults::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
}
