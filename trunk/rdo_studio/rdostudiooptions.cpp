#include "stdafx.h"
#include "rdostudiooptions.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

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
	ON_BN_CLICKED(IDC_CLEARAUTO_CHECK, OnClearAutoCheck)
	ON_BN_CLICKED(IDC_SHOWNEARESTWORDSONLY_RADIO, OnUpdateModify)
	ON_EN_CHANGE(IDC_CLEARAUTO_EDIT, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsEditor::RDOStudioOptionsEditor( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsEditor)
	m_bufferClearAuto = FALSE;
	m_bufferDelay = 0;
	m_codecompUse = FALSE;
	m_codecompShowFullList = -1;
	//}}AFX_DATA_INIT

	m_bufferClearAuto = sheet->style_editor.buffer->canClearBuffer ? 1 : 0;
	m_bufferDelay     = sheet->style_editor.buffer->clearBufferDelay;

	m_codecompUse          = sheet->style_editor.autoComplete->useAutoComplete ? 1 : 0;
	m_codecompShowFullList = sheet->style_editor.autoComplete->showFullList ? 0 : 1;
}

RDOStudioOptionsEditor::~RDOStudioOptionsEditor()
{
}

void RDOStudioOptionsEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioOptionsEditor)
	DDX_Check(pDX, IDC_CLEARAUTO_CHECK, m_bufferClearAuto);
	DDX_Text(pDX, IDC_CLEARAUTO_EDIT, m_bufferDelay);
	DDV_MinMaxInt(pDX, m_bufferDelay, 1, 100);
	DDX_Check(pDX, IDC_USEAUTOCOMPLETE_CHECK, m_codecompUse);
	DDX_Radio(pDX, IDC_SHOWFULLLIST_RADIO, m_codecompShowFullList);
	//}}AFX_DATA_MAP
}

BOOL RDOStudioOptionsEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	OnClearAutoCheck();
	OnUseAutoCompleteCheck();

	return TRUE;
}

void RDOStudioOptionsEditor::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsEditor::OnClearAutoCheck() 
{
	bool use = ((CButton*)GetDlgItem( IDC_CLEARAUTO_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_CLEARAUTO_STATIC1 )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_STATIC2 )->EnableWindow( use );
	OnUpdateModify();
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

	sheet->style_editor.buffer->canClearBuffer   = m_bufferClearAuto ? true : false;
	sheet->style_editor.buffer->clearBufferDelay = m_bufferDelay;

	sheet->style_editor.autoComplete->useAutoComplete = m_codecompUse ? true : false;
	sheet->style_editor.autoComplete->showFullList    = m_codecompShowFullList == 0;

//	if ( sheet->colorOptions->edit ) {
//		sheet->colorOptions->edit.setEditorStyle( sheet->editorStyle );
//	}

	SetModified( *sheet->style_editor.buffer != *studioApp.mainFrame->style_editor.buffer || *sheet->style_editor.autoComplete != *studioApp.mainFrame->style_editor.autoComplete );
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsTabs
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsTabs, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsTabs)
	ON_BN_CLICKED(IDC_USETABS_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_TABSIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_TABINDENTS_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_INDENTSIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_BACKSPACEUNTABS_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_BACKSPACEUNINDENTS_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_AUTOINDENT_CHECK, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsTabs::RDOStudioOptionsTabs( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsTabs)
	m_tabUse = FALSE;
	m_tabSize = 0;
	m_tabUseTabIndent = FALSE;
	m_tabIndentSize = 0;
	m_tabBackspaceUntabs = -1;
	m_tabAutoIndent = FALSE;
	//}}AFX_DATA_INIT

	m_tabUse             = sheet->style_editor.tab->useTabs ? 1 : 0;
	m_tabSize            = sheet->style_editor.tab->tabSize;
	m_tabUseTabIndent    = sheet->style_editor.tab->tabIndents ? 1 : 0;
	m_tabIndentSize      = sheet->style_editor.tab->indentSize;
	m_tabBackspaceUntabs = sheet->style_editor.tab->backspaceUntabs ? 0 : 1;
	m_tabAutoIndent      = sheet->style_editor.tab->autoIndent ? 1 : 0;
}

RDOStudioOptionsTabs::~RDOStudioOptionsTabs()
{
}

void RDOStudioOptionsTabs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioOptionsTabs)
	DDX_Check(pDX, IDC_USETABS_CHECK, m_tabUse);
	DDX_Text(pDX, IDC_TABSIZE_EDIT, m_tabSize);
	DDV_MinMaxInt(pDX, m_tabSize, 1, 100);
	DDX_Radio(pDX, IDC_BACKSPACEUNTABS_RADIO, m_tabBackspaceUntabs);
	DDX_Text(pDX, IDC_INDENTSIZE_EDIT, m_tabIndentSize);
	DDV_MinMaxInt(pDX, m_tabIndentSize, 1, 100);
	DDX_Check(pDX, IDC_AUTOINDENT_CHECK, m_tabAutoIndent);
	DDX_Check(pDX, IDC_TABINDENTS_CHECK, m_tabUseTabIndent);
	//}}AFX_DATA_MAP
}

void RDOStudioOptionsTabs::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsTabs::OnUpdateModify() 
{
	UpdateData();

	sheet->style_editor.tab->useTabs         = m_tabUse ? true : false;
	sheet->style_editor.tab->tabSize         = m_tabSize;
	sheet->style_editor.tab->tabIndents      = m_tabUseTabIndent ? true : false;
	sheet->style_editor.tab->indentSize      = m_tabIndentSize;
	sheet->style_editor.tab->backspaceUntabs = m_tabBackspaceUntabs == 0;
	sheet->style_editor.tab->autoIndent      = m_tabAutoIndent ? true : false;

	SetModified( *sheet->style_editor.tab != *studioApp.mainFrame->style_editor.tab );
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsStylesAndColors
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsStylesAndColors, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsStylesAndColors)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsStylesAndColors::RDOStudioOptionsStylesAndColors( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsStylesAndColors)
	//}}AFX_DATA_INIT
}

RDOStudioOptionsStylesAndColors::~RDOStudioOptionsStylesAndColors()
{
}

void RDOStudioOptionsStylesAndColors::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioOptionsStylesAndColors)
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}

BOOL RDOStudioOptionsStylesAndColors::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CComboBox*)GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	sheet->edit.Create( NULL, NULL, WS_CHILD | WS_VISIBLE, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->edit.setEditorStyle( &sheet->style_editor );
	CString s;
	s.LoadString( ID_OPTIONS_COLORS_EDITTEXT );
	sheet->edit.replaceCurrent( (LPCTSTR)s, 0 );
	sheet->edit.setReadOnly( true );
	sheet->edit.bookmarkToggle();

	CListBox* styleLB = (CListBox*)GetDlgItem( IDC_STYLE_LIST );
	if ( styleLB ) {
		CRect r;
		GetClientRect( r );

		CRect rectStyleLB;
		styleLB->GetWindowRect( rectStyleLB );
		ScreenToClient( rectStyleLB );

		CRect rectEdit;
		rectEdit.left   = 0;
		rectEdit.right  = r.right;
		rectEdit.top    = rectStyleLB.bottom + 5;
		rectEdit.bottom = r.bottom;

		sheet->edit.MoveWindow( rectEdit );
	}

	return true;
}

void RDOStudioOptionsStylesAndColors::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsStylesAndColors::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
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
	styles( NULL )
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

	editor = new RDOStudioOptionsEditor( *this );
	tabs   = new RDOStudioOptionsTabs( *this );
	styles = new RDOStudioOptionsStylesAndColors( *this );
	AddPage( editor );
	AddPage( tabs );
	AddPage( styles );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioOptions::~RDOStudioOptions()
{
	if ( editor )  { delete editor; editor = NULL; }
	if ( tabs )    { delete tabs;   tabs = NULL; }
	if ( styles )  { delete styles; styles = NULL; }
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
