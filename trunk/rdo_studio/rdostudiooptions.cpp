#include "stdafx.h"
#include "rdostudiooptions.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsSourceEditor
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsSourceEditor, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsSourceEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsSourceEditor::RDOStudioOptionsSourceEditor(): CPropertyPage( IDD_OPTIONS_EDITOR_DIALOG )
{
}

RDOStudioOptionsSourceEditor::~RDOStudioOptionsSourceEditor()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsStyleColor
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsStyleColor, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsStyleColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsStyleColor::RDOStudioOptionsStyleColor(): CPropertyPage( IDD_OPTIONS_COLORS_DIALOG )
{
}

RDOStudioOptionsStyleColor::~RDOStudioOptionsStyleColor()
{
}

void RDOStudioOptionsStyleColor::DoDataExchange(CDataExchange* pDX) 
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
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
	sourceEditor( NULL ),
	styleColor( NULL )
{
	SetTitle( format( ID_OPTIONS ).c_str() );

//	editorStyle = style;
//	prevStyle   = style;

	sourceEditor = new RDOStudioOptionsSourceEditor();
	styleColor   = new RDOStudioOptionsStyleColor();
	AddPage( sourceEditor );
	AddPage( styleColor );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioOptions::~RDOStudioOptions()
{
	if ( sourceEditor ) delete sourceEditor;
	if ( styleColor ) delete styleColor;
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

BOOL RDOStudioOptionsStyleColor::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CComboBox*)GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	return true;
}
