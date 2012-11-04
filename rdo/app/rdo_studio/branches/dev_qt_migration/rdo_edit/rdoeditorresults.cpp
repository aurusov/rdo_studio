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
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/resource.h"
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

//! @todo qt
//BEGIN_MESSAGE_MAP( RDOEditorResults, RDOEditorBaseEdit )
//	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
//	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
//END_MESSAGE_MAP()

RDOEditorResults::RDOEditorResults(PTR(QWidget) pParent)
	: RDOEditorBaseEdit(pParent)
{
	kw0 = "TRUE FALSE";
	kw1 = "";
	kw2 = "";
	kw3 = "";

	setReadOnly( true );
}

RDOEditorResults::~RDOEditorResults()
{}

void RDOEditorResults::setEditorStyle(PTR(RDOEditorResultsStyle) pStyle)
{
	RDOEditorBaseEdit::setEditorStyle(pStyle);
}

void RDOEditorResults::OnHelpKeyword()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if ( s.find_first_of( keyword ) == tstring::npos || keyword.empty() ) {
		keyword = "pmv";
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword.c_str());
	ba.append("\n");
	studioApp.callQtAssistant(ba);
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
