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
	ON_NOTIFY(TVN_SELCHANGED, IDC_STYLEITEM_TREE, OnStyleItemChanged)
	ON_CBN_SELCHANGE(IDC_PREVIEWAS_COMBO, OnPreviewAsChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsStylesAndColors::RDOStudioOptionsStylesAndColors( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	isCurrentFixed( false ),
	previewAs( STYLEObject::none )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsStylesAndColors)
	//}}AFX_DATA_INIT

	STYLEObject* object;
	object = new STYLEObject( STYLEObject::all );
	object->properties.push_back( new STYLEProperty( object, "All Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::source );
	object->properties.push_back( new STYLEProperty( object, "Source Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::build, false );
	object->properties.push_back( new STYLEProperty( object, "Build Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::debug, false );
	object->properties.push_back( new STYLEProperty( object, "Debug Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::trace );
	object->properties.push_back( new STYLEProperty( object, "Trace Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::results );
	object->properties.push_back( new STYLEProperty( object, "Results Windows" ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::find );
	object->properties.push_back( new STYLEProperty( object, "Find Windows" ) );
	objects.push_back( object );
}

RDOStudioOptionsStylesAndColors::~RDOStudioOptionsStylesAndColors()
{
	list< STYLEObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		delete *it;
		it++;
	};
}

void RDOStudioOptionsStylesAndColors::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioOptionsStylesAndColors)
	DDX_Control(pDX, IDC_PREVIEWAS_COMBO, m_previewAs);
	DDX_Control(pDX, IDC_FONTNAME_COMBO, m_fontName);
	DDX_Control(pDX, IDC_STYLEITEM_TREE, m_styleItem);
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}

int CALLBACK RDOStudioOptionsStylesAndColors::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam )
{
	list< STYLEFont >* fonts = reinterpret_cast< list< STYLEFont >* >(lParam);
	bool can_insert = true;
	list< STYLEFont >::iterator font_it = fonts->begin();
	while ( font_it != fonts->end() ) {
		if ( font_it->name == lpelfe->elfLogFont.lfFaceName ) {
			can_insert = false;
			break;
		}
		font_it++;
	}
	if ( can_insert ) {
		fonts->insert( fonts->end(), STYLEFont( lpelfe->elfLogFont.lfFaceName, lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH ) );
	}
	return 1;
}

BOOL RDOStudioOptionsStylesAndColors::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CComboBox*)GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	sheet->edit.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->edit.setEditorStyle( &sheet->style_editor );
	sheet->edit.replaceCurrent( format( ID_OPTIONS_COLORS_EDITTEXT ), 0 );
	sheet->edit.setReadOnly( true );
	sheet->edit.bookmarkToggle();

	sheet->build.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->build.setEditorStyle( &sheet->style_build );
	sheet->build.appendLine( new rdoEditCtrl::RDOBuildEditLineInfo( "Building Model..." ) );
	sheet->build.appendLine( new rdoEditCtrl::RDOBuildEditLineInfo( "Wrong parameter value: L", rdoModelObjects::PAT, 40, true ) );
	sheet->build.appendLine( new rdoEditCtrl::RDOBuildEditLineInfo( "1 error(s) found." ) );

	sheet->debug.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->debug.setEditorStyle( &sheet->style_debug );
	sheet->debug.appendLine( "Start executing\r\n" );
	sheet->debug.appendLine( "EI 0 1 1 0\r\n" );
	sheet->debug.appendLine( "ES 0 1\r\n" );

	sheet->tracer.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );

	sheet->results.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->results.setEditorStyle( &sheet->style_results );
	sheet->results.setReadOnly( false );
	sheet->results.replaceCurrent( format( ID_OPTIONS_COLOR_RESULTSTEXT ), 0 );
	sheet->results.setReadOnly( true );

	sheet->find.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->find.setEditorStyle( &sheet->style_find );
	sheet->find.setKeyword( "$Time" );
	sheet->find.appendLine( new rdoEditCtrl::RDOLogEditLineInfo( "Searching for '$Time'..." ) );
	sheet->find.appendLine( new rdoEditCtrl::RDOLogEditLineInfo( "$Time = Равномерный(0.25, 0.75)", rdoModelObjects::PAT, 3 ) );
	sheet->find.appendLine( new rdoEditCtrl::RDOLogEditLineInfo( "$Time = Нормальный(0.45, 0.2)", rdoModelObjects::PAT, 13 ) );
	sheet->find.appendLine( new rdoEditCtrl::RDOLogEditLineInfo( "'2' occurrence(s) have been found." ) );

	CRect r;
	GetClientRect( r );

	CRect rectStyleLB;
	m_styleItem.GetWindowRect( rectStyleLB );
	ScreenToClient( rectStyleLB );

	CRect rectEdit;
	rectEdit.left   = 0;
	rectEdit.right  = r.right;
	rectEdit.top    = rectStyleLB.bottom + 5;
	rectEdit.bottom = r.bottom;

	sheet->edit.MoveWindow( rectEdit );
	sheet->build.MoveWindow( rectEdit );
	sheet->debug.MoveWindow( rectEdit );
	sheet->tracer.MoveWindow( rectEdit );
	sheet->results.MoveWindow( rectEdit );
	sheet->find.MoveWindow( rectEdit );

	list< STYLEObject* >::iterator obj_it = objects.begin();
	while ( obj_it != objects.end() ) {
		list< STYLEProperty* >::iterator prop_it = (*obj_it)->properties.begin();
		HTREEITEM root = TVI_ROOT;
		while ( prop_it != (*obj_it)->properties.end() ) {
			if ( (*prop_it)->root ) {
				root = TVI_ROOT;
			}
			HTREEITEM item = m_styleItem.InsertItem( (*prop_it)->name.c_str(), root );
			m_styleItem.SetItemData( item, reinterpret_cast<DWORD>(*prop_it) );
			if ( (*prop_it)->root ) {
				root = item;
			}
			prop_it++;
		}
		obj_it++;
	}

	LOGFONT lf;
	lf.lfCharSet = sheet->style_editor.font->characterSet;
	lf.lfFaceName[0] = '\0';
	::EnumFontFamiliesEx( GetDC()->m_hDC, &lf, reinterpret_cast<FONTENUMPROC>(RDOStudioOptionsStylesAndColors::EnumFontFamExProc), reinterpret_cast<LPARAM>(&fonts), 0 );
	isCurrentFixed = false;
	loadFontsIntoCombo( true );

	setPreviewAsCombo( STYLEObject::source );

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

void RDOStudioOptionsStylesAndColors::loadFontsIntoCombo( bool fixed )
{
	if ( isCurrentFixed != fixed ) {
		m_fontName.ResetContent();
		list< STYLEFont >::iterator font_it = fonts.begin();
		while ( font_it != fonts.end() ) {
			if ( !fixed || ( fixed && font_it->fixed ) ) {
				m_fontName.AddString( font_it->name.c_str() );
			}
			font_it++;
		}
		isCurrentFixed = fixed;
	}
}

void RDOStudioOptionsStylesAndColors::OnStyleItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM item = m_styleItem.GetSelectedItem();
	if ( item ) {
		STYLEProperty* prop = reinterpret_cast<STYLEProperty*>(m_styleItem.GetItemData( item ));
		loadFontsIntoCombo( prop->object->fixedFont );
		setPreviewAsCombo( prop->object->type );
		switch ( prop->object->type ) {
			case STYLEObject::all: {
				break;
			}
			case STYLEObject::source: {
				break;
			}
			case STYLEObject::build: {
				break;
			}
			case STYLEObject::debug: {
				break;
			}
			case STYLEObject::trace: {
				break;
			}
			case STYLEObject::results: {
				break;
			}
			case STYLEObject::find: {
				break;
			}
			default: break;
		}
/*
		int index = m_fontName.FindStringExact( -1, sheet->editorStyle.font.name );
		if ( index != CB_ERR ) {
			fontNameCB->SetCurSel( index );
		} else {
			fontNameCB->SetCurSel( -1 );
		}
*/
	}

	*pResult = 0;
}

void RDOStudioOptionsStylesAndColors::OnPreviewAsChanged()
{
	int index = m_previewAs.GetCurSel();
	if ( index != CB_ERR ) {
		if ( index == 0 ) {
			setPreviewAsCombo( STYLEObject::source );
		} else if ( index == 1 ) {
			setPreviewAsCombo( STYLEObject::build );
		} else if ( index == 2 ) {
			setPreviewAsCombo( STYLEObject::debug );
		} else if ( index == 3 ) {
			setPreviewAsCombo( STYLEObject::trace );
		} else if ( index == 4 ) {
			setPreviewAsCombo( STYLEObject::results );
		} else if ( index == 5 ) {
			setPreviewAsCombo( STYLEObject::find );
		}
	}
}

void RDOStudioOptionsStylesAndColors::setPreviewAsCombo( STYLEObject::Type type )
{
	if ( previewAs != type && type >= STYLEObject::source ) {
		previewAs = type;
		sheet->edit.ShowWindow( SW_HIDE );
		sheet->build.ShowWindow( SW_HIDE );
		sheet->debug.ShowWindow( SW_HIDE );
		sheet->tracer.ShowWindow( SW_HIDE );
		sheet->results.ShowWindow( SW_HIDE );
		sheet->find.ShowWindow( SW_HIDE );
		switch ( previewAs ) {
			case STYLEObject::source: {
				m_previewAs.SetCurSel( 0 );
				sheet->edit.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::build: {
				m_previewAs.SetCurSel( 1 );
				sheet->build.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::debug: {
				m_previewAs.SetCurSel( 2 );
				sheet->debug.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::trace: {
				m_previewAs.SetCurSel( 3 );
				sheet->tracer.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::results: {
				m_previewAs.SetCurSel( 4 );
				sheet->results.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::find: {
				m_previewAs.SetCurSel( 5 );
				sheet->find.ShowWindow( SW_SHOW );
				break;
			}
			default: break;
		}
	}
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
