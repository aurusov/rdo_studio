#include "stdafx.h"
#include "rdostudiooptions.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsEditor
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsEditor, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsEditor)
	ON_BN_CLICKED(IDC_USEAUTOCOMPLETE_CHECK, OnUseAutoCompleteCheck)
	ON_BN_CLICKED(IDC_SHOWFULLLIST_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_SHOWNEARESTWORDSONLY_RADIO, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsEditor::RDOStudioOptionsEditor( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD_OPTIONS_EDITOR ),
	sheet( &_sheet )
{
	useAutoComplete = sheet->style_editor.autoComplete->useAutoComplete ? 1 : 0;
	showFullList    = sheet->style_editor.autoComplete->showFullList ? 0 : 1;
}

RDOStudioOptionsEditor::~RDOStudioOptionsEditor()
{
}

void RDOStudioOptionsEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check( pDX, IDC_USEAUTOCOMPLETE_CHECK, useAutoComplete );
	DDX_Radio( pDX, IDC_SHOWFULLLIST_RADIO   , showFullList );
}

BOOL RDOStudioOptionsEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	OnUseAutoCompleteCheck();

	return TRUE;
}

void RDOStudioOptionsEditor::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsEditor::OnUseAutoCompleteCheck() 
{
	bool use = ((CButton*)GetDlgItem( IDC_USEAUTOCOMPLETE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_SHOWFULLLIST_RADIO )->EnableWindow( use );
	GetDlgItem( IDC_SHOWNEARESTWORDSONLY_RADIO )->EnableWindow( use );
	OnUpdateModify();
}

void RDOStudioOptionsEditor::OnUpdateModify() 
{
	UpdateData();

	sheet->style_editor.autoComplete->useAutoComplete = useAutoComplete ? true : false;
	sheet->style_editor.autoComplete->showFullList    = showFullList == 0;

//	if ( sheet->colorOptions->edit ) {
//		sheet->colorOptions->edit.setEditorStyle( sheet->editorStyle );
//	}

	SetModified( *sheet->style_editor.autoComplete != *studioApp.mainFrame->style_editor.autoComplete /*|| canClearBuffer != prev_canClearBuffer || clearBufferDelay != prev_clearBufferDelay*/ );
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsTabs
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsTabs, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsTabs::RDOStudioOptionsTabs( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD_OPTIONS_TABS ),
	sheet( &_sheet )
{
}

RDOStudioOptionsTabs::~RDOStudioOptionsTabs()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsColorsAndStyles
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsColorsAndStyles, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsColorsAndStyles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsColorsAndStyles::RDOStudioOptionsColorsAndStyles( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD_OPTIONS_COLORSANDSTYLES ),
	sheet( &_sheet )
{
}

RDOStudioOptionsColorsAndStyles::~RDOStudioOptionsColorsAndStyles()
{
}

void RDOStudioOptionsColorsAndStyles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}


BOOL RDOStudioOptionsColorsAndStyles::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CComboBox*)GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	return true;
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
	editor( NULL ),
	tabs( NULL ),
	colorsAndStyles( NULL )
{
	SetTitle( format( ID_OPTIONS ).c_str() );

	style_editor.init();
	style_build.init();
	style_debug.init();
	style_results.init();
	style_find.init();

	style_editor  = studioApp.mainFrame->style_editor;
	style_build   = studioApp.mainFrame->style_build;
	style_debug   = studioApp.mainFrame->style_debug;
	style_results = studioApp.mainFrame->style_results;
	style_find    = studioApp.mainFrame->style_find;

	editor          = new RDOStudioOptionsEditor( *this );
	tabs            = new RDOStudioOptionsTabs( *this );
	colorsAndStyles = new RDOStudioOptionsColorsAndStyles( *this );
	AddPage( editor );
	AddPage( tabs );
	AddPage( colorsAndStyles );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioOptions::~RDOStudioOptions()
{
	if ( editor ) delete editor;
	if ( tabs ) delete tabs;
	if ( colorsAndStyles ) delete colorsAndStyles;
}

void RDOStudioOptions::apply()
{
	studioApp.mainFrame->style_editor  = style_editor;
	studioApp.mainFrame->style_build   = style_build;
	studioApp.mainFrame->style_debug   = style_debug;
	studioApp.mainFrame->style_results = style_results;
	studioApp.mainFrame->style_find    = style_find;
	studioApp.mainFrame->updateAllStyles();
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
