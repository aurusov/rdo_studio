#include "stdafx.h"
#include "rdostudiooptions.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioSourceEditorOptions
// ----------------------------------------------------------------------------
RDOStudioSourceEditorOptions::RDOStudioSourceEditorOptions(): CPropertyPage( IDD_OPTIONS_EDITOR_DIALOG )
{
}

RDOStudioSourceEditorOptions::~RDOStudioSourceEditorOptions()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioStyleColorOptions
// ----------------------------------------------------------------------------
RDOStudioStyleColorOptions::RDOStudioStyleColorOptions(): CPropertyPage( IDD_OPTIONS_COLORS_DIALOG )
{
}

RDOStudioStyleColorOptions::~RDOStudioStyleColorOptions()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptions, CPropertySheet)
	//{{AFX_MSG_MAP(RDOStudioOptions)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptions::RDOStudioOptions():
	CPropertySheet(),
	sourceEditorOptions( NULL ),
	styleColorOptions( NULL )
{
	SetTitle( format( ID_OPTIONS ).c_str() );

//	editorStyle = style;
//	prevStyle   = style;

	sourceEditorOptions = new RDOStudioSourceEditorOptions();
	styleColorOptions   = new RDOStudioStyleColorOptions();
	AddPage( sourceEditorOptions );
	AddPage( styleColorOptions );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioOptions::~RDOStudioOptions()
{
	if ( sourceEditorOptions ) delete sourceEditorOptions;
	if ( styleColorOptions ) delete styleColorOptions;
}

void RDOStudioOptions::apply()
{
/*
	RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
	int itemCount = tabCtrl->getItemCount();
	for ( int i = 0; i < itemCount; i++ ) {
		tabCtrl->getItemEdit( i )->setEditorStyle( editorStyle );
	}
	editorStyle.save();
	prevStyle = editorStyle;
	colorOptions->currentTheme = editorStyle.theme;
*/
}

int CALLBACK RDOStudioOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam)
{
	switch (message) {
		case PSCB_INITIALIZED: {
			LONG style = ::GetWindowLong( hwnd, GWL_EXSTYLE );
			style |= WS_EX_CONTEXTHELP;
			::SetWindowLong( hwnd, GWL_EXSTYLE, style );
		}
	}
	return true;
}

void RDOStudioOptions::onHelpButton()
{
//	CString filename = rdoEditorApp.getFullHelpFileName();
//	if ( filename.IsEmpty() ) return;

//	CPropertyPage* page = GetActivePage( );
//	if ( page == editorOptions )
//		filename += "::/html/work_options.htm#edit";
//	if ( page == colorOptions )
//		filename += "::/html/work_options.htm#color";
//	HtmlHelp( ::GetDesktopWindow(), filename, HH_DISPLAY_TOPIC, NULL );
}

BOOL RDOStudioOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
//	CString filename = rdoEditorApp.getFullHelpFileName();
//	if ( filename.IsEmpty() ) return TRUE;

//	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
//		return HtmlHelp( ::GetDesktopWindow(), filename, HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
