#include "stdafx.h"
//#include "rdoeditoroptions.h"
//#include "rdoeditorproject.h"
//#include "../resource.h"
//#include "../htmlhelp.h"
//#include "rdoeditorapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/*
// ----------------------------------------------------------------------------
// ---------- RDOEditorEditorOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorEditorOptions, CPropertyPage )

	ON_EN_CHANGE( IDC_TABSIZE_EDIT               , OnUpdateModify )
	ON_EN_CHANGE( IDC_INDENTSIZE_EDIT            , OnUpdateModify )
	ON_BN_CLICKED( IDC_USETABS_CHECK             , OnUpdateModify )
	ON_BN_CLICKED( IDC_TABINDENTS_CHECK          , OnUpdateModify )
	ON_BN_CLICKED( IDC_BACKSPACEUNTABS_RADIO     , OnUpdateModify )
	ON_BN_CLICKED( IDC_BACKSPACEUNINDENTS_RADIO  , OnUpdateModify )
	ON_BN_CLICKED( IDC_AUTOINDENT_CHECK          , OnUpdateModify )

	ON_BN_CLICKED( IDC_USEAUTOCOMPLETE_CHECK     , OnUseAutoCompleteCBChanged )
	ON_BN_CLICKED( IDC_SHOWFULLLIST_RADIO        , OnUpdateModify )
	ON_BN_CLICKED( IDC_SHOWNEARESTWORDSONLY_RADIO, OnUpdateModify )

	ON_BN_CLICKED( IDC_DEBUGNONE_RADIO               , OnUpdateModify )
	ON_BN_CLICKED( IDC_DEBUGHOOK_RADIO               , OnUpdateModify )
	ON_BN_CLICKED( IDC_DEBUGTIMER_RADIO              , OnUpdateModify )
	ON_BN_CLICKED( IDC_DEBUGWAITFORSINGLEOBJECT_RADIO, OnUpdateModify )

	ON_BN_CLICKED( IDC_CLEARAUTO_CHECK, OnUseAutoClearBufferCBChanged )
	ON_EN_CHANGE( IDC_CLEARAUTO_EDIT  , OnUpdateModify )

	ON_BN_CLICKED( IDC_WORDWRAP_CHECK            , OnUseWordWrapCBChanged )
	ON_BN_CLICKED( IDC_HORZSCROLLBAR_CHECK       , OnUpdateModify )

END_MESSAGE_MAP()

RDOEditorEditorOptions::RDOEditorEditorOptions( RDOEditorOptions& _sheet, RDODebug _debug, bool _canClearBuffer, int _clearBufferDelay ):
	CPropertyPage( IDD_OPTIONS_EDITOR_DIALOG ),
	sheet( &_sheet ),
	clearBufferDelay( _clearBufferDelay ),
	prev_clearBufferDelay( _clearBufferDelay )
{
	RDOEditorEditTab* tab = &sheet->editorStyle.tab;
	tabSize         = tab->tabSize;
	indentSize      = tab->indentSize;
	useTabs         = tab->useTabs ? 1 : 0;
	tabIndents      = tab->tabIndents ? 1 : 0;
	backspaceUntabs = tab->backspaceUntabs ? 0 : 1;
	autoIndent      = tab->autoIndent ? 1 : 0;

	useAutoComplete = sheet->editorStyle.autoComplete.useAutoComplete ? 1 : 0;
	showFullList    = sheet->editorStyle.autoComplete.showFullList ? 0 : 1;

	debug      = (int)_debug;
	prev_debug = debug;

	canClearBuffer      = _canClearBuffer ? 1 : 0;
	prev_canClearBuffer = canClearBuffer;

	wordWrap          = sheet->editorStyle.window.wordWrap ? 1 : 0;
	showHorzScrollBar = sheet->editorStyle.window.showHorzScrollBar ? 1 : 0;

	m_psp.dwFlags |= PSP_HASHELP;
}

void RDOEditorEditorOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Text(  pDX, IDC_TABSIZE_EDIT         , tabSize );
	DDX_Text(  pDX, IDC_INDENTSIZE_EDIT      , indentSize );
	DDX_Check( pDX, IDC_USETABS_CHECK        , useTabs );
	DDX_Check( pDX, IDC_TABINDENTS_CHECK     , tabIndents );
	DDX_Radio( pDX, IDC_BACKSPACEUNTABS_RADIO, backspaceUntabs );
	DDX_Check( pDX, IDC_AUTOINDENT_CHECK     , autoIndent );

	DDX_Check( pDX, IDC_USEAUTOCOMPLETE_CHECK, useAutoComplete );
	DDX_Radio( pDX, IDC_SHOWFULLLIST_RADIO   , showFullList );

	DDX_Radio( pDX, IDC_DEBUGNONE_RADIO, debug );

	DDX_Check( pDX, IDC_CLEARAUTO_CHECK, canClearBuffer );
	DDX_Text(  pDX, IDC_CLEARAUTO_EDIT , clearBufferDelay );

	DDX_Check( pDX, IDC_WORDWRAP_CHECK     , wordWrap );
	DDX_Check( pDX, IDC_HORZSCROLLBAR_CHECK, showHorzScrollBar );
}

BOOL RDOEditorEditorOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	((CEdit*)GetDlgItem( IDC_TABSIZE_EDIT ))->SetLimitText( 2 );
	((CEdit*)GetDlgItem( IDC_INDENTSIZE_EDIT ))->SetLimitText( 2 );
	((CEdit*)GetDlgItem( IDC_CLEARAUTO_EDIT ))->SetLimitText( 2 );

	((CButton*)GetDlgItem( IDC_DEBUGHOOK_RADIO ))->EnableWindow( ((RDOEditorMainFrame*)AfxGetMainWnd())->canHook() );

	OnUseAutoCompleteCBChanged();
	OnUseAutoClearBufferCBChanged();
	OnUseWordWrapCBChanged();

	return true;
}

void RDOEditorEditorOptions::OnOK()
{
	sheet->apply();

	RDOEditorMainFrame* mainFrame = (RDOEditorMainFrame*)AfxGetMainWnd();

	mainFrame->setDebug( (RDODebug)debug );
	prev_debug = debug;

	rdoEditorProject.canClearBuffer   = canClearBuffer ? true : false;
	rdoEditorProject.clearBufferDelay = clearBufferDelay;
	mainFrame->restartBufTimer( 1 );
	mainFrame->restartBufTimer( 2 );
	mainFrame->restartBufTimer( 3 );
	mainFrame->restartBufTimer( 4 );
	if ( !canClearBuffer ) {
		rdoEditorProject.resetBuf1 = false;
		rdoEditorProject.resetBuf2 = false;
		rdoEditorProject.resetBuf3 = false;
		rdoEditorProject.resetBuf4 = false;
	}
	prev_canClearBuffer   = canClearBuffer;
	prev_clearBufferDelay = clearBufferDelay;

	CPropertyPage::OnOK();
}

BOOL RDOEditorEditorOptions::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( ((LPNMHDR) lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify( wParam, lParam, pResult );
}

void RDOEditorEditorOptions::OnUseAutoCompleteCBChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_USEAUTOCOMPLETE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_SHOWFULLLIST_RADIO )->EnableWindow( use );
	GetDlgItem( IDC_SHOWNEARESTWORDSONLY_RADIO )->EnableWindow( use );
	OnUpdateModify();
}

void RDOEditorEditorOptions::OnUseAutoClearBufferCBChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_CLEARAUTO_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_CLEARAUTO_STATIC1 )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_STATIC2 )->EnableWindow( use );
	OnUpdateModify();
}

void RDOEditorEditorOptions::OnUseWordWrapCBChanged()
{
	GetDlgItem( IDC_HORZSCROLLBAR_CHECK )->EnableWindow( !((CButton*)GetDlgItem( IDC_WORDWRAP_CHECK ))->GetCheck() );
	OnUpdateModify();
}

void RDOEditorEditorOptions::OnUpdateModify()
{
	UpdateData();

	RDOEditorEditTab* tab = &sheet->editorStyle.tab;
	if ( tabSize ) {
		tab->tabSize = tabSize;
	}
	if ( indentSize ) {
		tab->indentSize = indentSize;
	}
	tab->useTabs         = useTabs ? true : false;
	tab->tabIndents      = tabIndents ? true : false;
	tab->backspaceUntabs = backspaceUntabs == 0;
	tab->autoIndent      = autoIndent ? true : false;

	sheet->editorStyle.autoComplete.useAutoComplete = useAutoComplete ? true : false;
	sheet->editorStyle.autoComplete.showFullList    = showFullList == 0;

	sheet->editorStyle.window.wordWrap          = wordWrap ? true : false;
	sheet->editorStyle.window.showHorzScrollBar = showHorzScrollBar ? true : false;

	if ( sheet->colorOptions->edit ) {
		sheet->colorOptions->edit.setEditorStyle( sheet->editorStyle );
	}

	SetModified( sheet->editorStyle.tab != sheet->prevStyle.tab || sheet->editorStyle.autoComplete != sheet->prevStyle.autoComplete || debug != prev_debug || canClearBuffer != prev_canClearBuffer || clearBufferDelay != prev_clearBufferDelay || sheet->editorStyle.window != sheet->prevStyle.window );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorColorsOptions
// ----------------------------------------------------------------------------
enum RDOThemeItem {
	RDOTI_NONE    = -1,
	RDOTI_CURRENT = 0,
	RDOTI_DEFAULT = 1,
	RDOTI_CPP,
	RDOTI_PASCAL,
	RDOTI_HTML,
	RDOTI_CLASSIC,
	RDOTI_TWILIGHT,
	RDOTI_OCEAN
};

enum RDOStyleItem {
	RDOSI_PLAINTEXT = 0,
	RDOSI_IDENTIFICATOR,
	RDOSI_KEYWORD,
	RDOSI_FUNCTION,
	RDOSI_TRACE,
	RDOSI_COMMENT,
	RDOSI_NUMBER,
	RDOSI_STRING,
	RDOSI_OPERATOR,
	RDOSI_CARET,
	RDOSI_TEXTSELECTION,
	RDOSI_BOOKMARK,
	RDOSI_FOLD,
	RDOSI_ERROR
};

BEGIN_MESSAGE_MAP( RDOEditorColorsOptions, CPropertyPage )

	ON_WM_SIZE()

	ON_CBN_SELCHANGE( IDC_THEME_COMBO   , OnThemeCBChanged )
	ON_LBN_SELCHANGE( IDC_STYLE_LIST    , OnStyleLBChanged )
	ON_CBN_SELCHANGE( IDC_FONTNAME_COMBO, OnFontNameCBChanged )
	ON_CBN_SELCHANGE( IDC_FONTSIZE_COMBO, OnFontSizeCBChanged )

	ON_BN_CLICKED( IDC_FONTBOLD_CHECK     , OnFontBoldBClick )
	ON_BN_CLICKED( IDC_FONTITALIC_CHECK   , OnFontItalicBClick )
	ON_BN_CLICKED( IDC_FONTUNDERLINE_CHECK, OnFontUnderlineBClick )

	ON_CBN_SELCHANGE( IDC_FGCOLOR_COMBO, OnFgColorCBChanged )
	ON_CBN_SELCHANGE( IDC_BGCOLOR_COMBO, OnBgColorCBChanged )
	ON_BN_CLICKED( IDC_FGCOLOR_BUTTON  , OnFgColorBClick )
	ON_BN_CLICKED( IDC_BGCOLOR_BUTTON  , OnBgColorBClick )

	ON_CBN_SELCHANGE( IDC_BOOKMARK_COMBO, OnBookmarkStyleCBChanged )
	ON_CBN_SELCHANGE( IDC_FOLD_COMBO    , OnFoldStyleCBChanged )

END_MESSAGE_MAP()

RDOEditorColorsOptions::RDOEditorColorsOptions( RDOEditorOptions& _sheet ):
	CPropertyPage( IDD_OPTIONS_COLORS_DIALOG ),
	sheet( &_sheet )
{
		m_psp.dwFlags |= PSP_HASHELP;
}

void RDOEditorColorsOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}

int CALLBACK RDOEditorColorsOptions::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam )
{
	if ( lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH ) {
		CStringList* tmpFontList = (CStringList*)lParam;
		if ( !tmpFontList->Find( (char*)lpelfe->elfLogFont.lfFaceName ) ) tmpFontList->AddTail( lpelfe->elfLogFont.lfFaceName );
	}
	return 1;
}

BOOL RDOEditorColorsOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CComboBox*)GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	edit.Create( NULL, NULL, WS_CHILD | WS_VISIBLE, CRect( 0, 0, 444, 223 ), this, -1 );
	edit.setEditorStyle( sheet->editorStyle );
	CString s;
	s.LoadString( ID_OPTIONS_COLORS_EDITTEXT );
	edit.replaceCurrent( s, 0 );
	edit.setReadOnly( true );
	edit.bookmarkToggle();

	currentTheme  = sheet->editorStyle.theme;
	defaultTheme  = RDOEditorEditTheme::getDefaultTheme();
	cppTheme      = RDOEditorEditTheme::getCppTheme();
	pascalTheme   = RDOEditorEditTheme::getPascalTheme();
	htmlTheme     = RDOEditorEditTheme::getHtmlTheme();
	classicTheme  = RDOEditorEditTheme::getClassicTheme();
	twilightTheme = RDOEditorEditTheme::getTwilightTheme();
	oceanTheme    = RDOEditorEditTheme::getOceanTheme();

	CComboBox* themeCB = (CComboBox*)GetDlgItem( IDC_THEME_COMBO );
	if ( themeCB ) {
		int i = RDOTI_NONE;
		if ( sheet->editorStyle.theme == defaultTheme ) {
			i = RDOTI_DEFAULT;
		} else if ( sheet->editorStyle.theme == cppTheme ) {
			i = RDOTI_CPP;
		} else if ( sheet->editorStyle.theme == pascalTheme ) {
			i = RDOTI_PASCAL;
		} else if ( sheet->editorStyle.theme == htmlTheme ) {
			i = RDOTI_HTML;
		} else if ( sheet->editorStyle.theme == classicTheme ) {
			i = RDOTI_CLASSIC;
		} else if ( sheet->editorStyle.theme == twilightTheme ) {
			i = RDOTI_TWILIGHT;
		} else if ( sheet->editorStyle.theme == oceanTheme ) {
			i = RDOTI_OCEAN;
		} else if ( sheet->editorStyle.theme == currentTheme ) {
			i = RDOTI_CURRENT;
		}
		themeCB->SetCurSel( i );
	}

	CListBox* styleLB = (CListBox*)GetDlgItem( IDC_STYLE_LIST );
	if ( styleLB ) {
		CString s;
		for ( int i = ID_COLORSTYLE_PLAINTEXT; i <= ID_COLORSTYLE_ERROR; i++ ) {
			s.LoadString( i );
			styleLB->AddString( s );
		}
		styleLB->SetCurSel( 0 );
	}

	LOGFONT lf;
	lf.lfCharSet = sheet->editorStyle.font.characterSet;
	lf.lfFaceName[0] = '\0';

	fontList.RemoveAll();
	::EnumFontFamiliesEx( GetDC()->m_hDC, &lf, (FONTENUMPROC)RDOEditorColorsOptions::EnumFontFamExProc, (LPARAM)&fontList, 0 );

	CComboBox* fontNameCB = (CComboBox*)GetDlgItem( IDC_FONTNAME_COMBO );
	if ( fontNameCB ) {
		POSITION pos;
		for( pos = fontList.GetHeadPosition(); pos != NULL; )
			fontNameCB->AddString( fontList.GetNext(pos) );

		int index = fontNameCB->FindStringExact( -1, sheet->editorStyle.font.name );
		if ( index != CB_ERR ) {
			fontNameCB->SetCurSel( index );
		} else {
			fontNameCB->SetCurSel( -1 );
		}
	}

	CComboBox* fontSizeCB = (CComboBox*)GetDlgItem( IDC_FONTSIZE_COMBO );
	if ( fontSizeCB ) {
		CString s;
		s.Format( "%d", sheet->editorStyle.font.size );
		int index = fontSizeCB->FindStringExact( -1, s );
		if ( index != CB_ERR ) {
			fontSizeCB->SetCurSel( index );
		} else {
			fontSizeCB->SetCurSel( -1 );
		}
	}

	OnStyleLBChanged();
	updateBookmarkStyle();
	updateFoldStyle();

	return true;
}

void RDOEditorColorsOptions::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOEditorColorsOptions::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( ((LPNMHDR) lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify( wParam, lParam, pResult );
}

void RDOEditorColorsOptions::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize( nType, cx, cy );

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

		edit.MoveWindow( rectEdit );
	}

	CButton* fgColorButton = (CButton*)GetDlgItem( IDC_FGCOLOR_BUTTON );
	if ( fgColorButton ) {
		CRect rectFgColorCB;
		fgColorCB.GetWindowRect( rectFgColorCB );
		ScreenToClient( rectFgColorCB );

		CRect rectFgColorButton;
		fgColorButton->GetWindowRect( rectFgColorButton );
		ScreenToClient( rectFgColorButton );

		rectFgColorButton.top    = rectFgColorCB.top;
		rectFgColorButton.bottom = rectFgColorCB.bottom;
		fgColorButton->MoveWindow( rectFgColorButton );
	}

	CButton* bgColorButton = (CButton*)GetDlgItem( IDC_BGCOLOR_BUTTON );
	if ( bgColorButton ) {
		CRect rectBgColorCB;
		bgColorCB.GetWindowRect( rectBgColorCB );
		ScreenToClient( rectBgColorCB );

		CRect rectBgColorButton;
		bgColorButton->GetWindowRect( rectBgColorButton );
		ScreenToClient( rectBgColorButton );

		rectBgColorButton.top    = rectBgColorCB.top;
		rectBgColorButton.bottom = rectBgColorCB.bottom;
		bgColorButton->MoveWindow( rectBgColorButton );
	}
}

void RDOEditorColorsOptions::OnThemeCBChanged()
{
	CComboBox* themeCB = (CComboBox*)GetDlgItem( IDC_THEME_COMBO );
	if ( themeCB ) {
		switch ( themeCB->GetCurSel() ) {
			case RDOTI_CURRENT : sheet->editorStyle.theme = currentTheme; break;
			case RDOTI_DEFAULT : sheet->editorStyle.theme = defaultTheme; break;
			case RDOTI_CPP     : sheet->editorStyle.theme = cppTheme; break;
			case RDOTI_PASCAL  : sheet->editorStyle.theme = pascalTheme; break;
			case RDOTI_HTML    : sheet->editorStyle.theme = htmlTheme; break;
			case RDOTI_CLASSIC : sheet->editorStyle.theme = classicTheme; break;
			case RDOTI_TWILIGHT: sheet->editorStyle.theme = twilightTheme; break;
			case RDOTI_OCEAN   : sheet->editorStyle.theme = oceanTheme; break;
		}
	}
	OnStyleLBChanged();
	updateBookmarkStyle();
	updateFoldStyle();
	updateEdit();
}

void RDOEditorColorsOptions::OnStyleLBChanged()
{
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;

	COLORREF fg = theme->defaultColor;
	COLORREF bg = theme->backgroundColor;
	RDOFontStyle fontStyle = theme->defaultStyle;
	bool enableFg = true;
	bool enableBg = true;
	bool enableFS = true;
	bool showFg   = true;
	bool showBg   = true;

	switch ( ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel() ) {
		case RDOSI_PLAINTEXT:
			break;
		case RDOSI_IDENTIFICATOR:
			fg        = theme->identifierColor;
			fontStyle = theme->identifierStyle;
			enableBg  = false;
			break;
		case RDOSI_KEYWORD:
			fg        = theme->keywordColor;
			fontStyle = theme->keywordStyle;
			enableBg  = false;
			break;
		case RDOSI_FUNCTION:
			fg        = theme->functionsColor;
			fontStyle = theme->functionsStyle;
			enableBg  = false;
			break;
		case RDOSI_TRACE:
			fg        = theme->traceColor;
			fontStyle = theme->traceStyle;
			enableBg  = false;
			break;
		case RDOSI_COMMENT:
			fg        = theme->commentColor;
			fontStyle = theme->commentStyle;
			enableBg  = false;
			break;
		case RDOSI_NUMBER:
			fg        = theme->numberColor;
			fontStyle = theme->numberStyle;
			enableBg  = false;
			break;
		case RDOSI_STRING:
			fg        = theme->stringColor;
			fontStyle = theme->stringStyle;
			enableBg  = false;
			break;
		case RDOSI_OPERATOR:
			fg        = theme->operatorColor;
			fontStyle = theme->operatorStyle;
			enableBg  = false;
			break;
		case RDOSI_CARET:
			fg        = theme->caretColor;
			enableBg  = false;
			enableFS  = false;
			showBg    = false;
			break;
		case RDOSI_TEXTSELECTION:
			bg        = theme->selectionBgColor;
			enableFg  = false;
			enableFS  = false;
			showFg    = false;
			break;
		case RDOSI_BOOKMARK:
			fg        = theme->bookmarkFgColor;
			bg        = theme->bookmarkBgColor;
			enableFS  = false;
			break;
		case RDOSI_FOLD:
			fg        = theme->foldFgColor;
			bg        = theme->foldBgColor;
			enableFS  = false;
			break;
		case RDOSI_ERROR:
			bg        = theme->errorBgColor;
			enableFg  = false;
			enableFS  = false;
			showFg    = false;
			break;
	}

	fgColorCB.EnableWindow( enableFg );
	bgColorCB.EnableWindow( enableBg );
	GetDlgItem( IDC_FGCOLOR_BUTTON )->EnableWindow( enableFg );
	GetDlgItem( IDC_BGCOLOR_BUTTON )->EnableWindow( enableBg );

	if ( showFg ) {
		fgColorCB.insertColor( fg );
	} else {
		fgColorCB.SetCurSel( -1 );
	}
	if ( showBg ) {
		bgColorCB.insertColor( bg );
	} else {
		bgColorCB.SetCurSel( -1 );
	}

	CButton* fontBoldB = (CButton*)GetDlgItem( IDC_FONTBOLD_CHECK );
	fontBoldB->EnableWindow( enableFS );
	if ( enableFS ) {
		fontBoldB->SetCheck( fontStyle & RDOFS_BOLD );
	} else {
		fontBoldB->SetCheck( false );
	}

	CButton* fontItalicB = (CButton*)GetDlgItem( IDC_FONTITALIC_CHECK );
	fontItalicB->EnableWindow( enableFS );
	if ( enableFS ) {
		fontItalicB->SetCheck( fontStyle & RDOFS_ITALIC );
	} else {
		fontItalicB->SetCheck( false );
	}

	CButton* fontUnderlineB = (CButton*)GetDlgItem( IDC_FONTUNDERLINE_CHECK );
	fontUnderlineB->EnableWindow( enableFS );
	if ( enableFS ) {
		fontUnderlineB->SetCheck( fontStyle & RDOFS_UNDERLINE );
	} else {
		fontUnderlineB->SetCheck( false );
	}
}

void RDOEditorColorsOptions::OnFontNameCBChanged()
{
	CComboBox* fontNameCB = (CComboBox*)GetDlgItem( IDC_FONTNAME_COMBO );
	if ( fontNameCB ) {
		int index = fontNameCB->GetCurSel();
		if ( index != CB_ERR ) {
			fontNameCB->GetLBText( index, sheet->editorStyle.font.name );
		}
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFontSizeCBChanged()
{
	CComboBox* fontSizeCB = (CComboBox*)GetDlgItem( IDC_FONTSIZE_COMBO );
	if ( fontSizeCB ) {
		int index = fontSizeCB->GetCurSel();
		if ( index != CB_ERR ) {
			CString s;
			fontSizeCB->GetLBText( index, s );
			sheet->editorStyle.font.size = strtol( s, NULL, 10 );
		}
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFontBoldBClick()
{
	bool bold = ((CButton*)GetDlgItem( IDC_FONTBOLD_CHECK ))->GetCheck() ? true : false;
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;
	int styleItem = ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel();

	RDOFontStyle fontStyle;
	if ( bold ) {
		fontStyle = RDOFS_BOLD;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle | fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle | fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle | fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle | fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle | fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle | fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle | fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle | fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle | fontStyle); break;
		}
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_BOLD;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle & fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle & fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle & fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle & fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle & fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle & fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle & fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle & fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle & fontStyle); break;
		}
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFontItalicBClick()
{
	bool italic = ((CButton*)GetDlgItem( IDC_FONTITALIC_CHECK ))->GetCheck() ? true : false;
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;
	int styleItem = ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel();

	RDOFontStyle fontStyle;
	if ( italic ) {
		fontStyle = RDOFS_ITALIC;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle | fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle | fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle | fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle | fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle | fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle | fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle | fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle | fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle | fontStyle); break;
		}
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_ITALIC;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle & fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle & fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle & fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle & fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle & fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle & fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle & fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle & fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle & fontStyle); break;
		}
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFontUnderlineBClick()
{
	bool underline = ((CButton*)GetDlgItem( IDC_FONTUNDERLINE_CHECK ))->GetCheck() ? true : false;
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;
	int styleItem = ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel();

	RDOFontStyle fontStyle;
	if ( underline ) {
		fontStyle = RDOFS_UNDERLINE;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle | fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle | fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle | fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle | fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle | fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle | fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle | fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle | fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle | fontStyle); break;
		}
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_UNDERLINE;
		switch ( styleItem ) {
			case RDOSI_PLAINTEXT    : theme->defaultStyle    = (RDOFontStyle)(theme->defaultStyle & fontStyle); break;
			case RDOSI_IDENTIFICATOR: theme->identifierStyle = (RDOFontStyle)(theme->identifierStyle & fontStyle); break;
			case RDOSI_KEYWORD      : theme->keywordStyle    = (RDOFontStyle)(theme->keywordStyle & fontStyle); break;
			case RDOSI_FUNCTION     : theme->functionsStyle  = (RDOFontStyle)(theme->functionsStyle & fontStyle); break;
			case RDOSI_TRACE        : theme->traceStyle      = (RDOFontStyle)(theme->traceStyle & fontStyle); break;
			case RDOSI_COMMENT      : theme->commentStyle    = (RDOFontStyle)(theme->commentStyle & fontStyle); break;
			case RDOSI_NUMBER       : theme->numberStyle     = (RDOFontStyle)(theme->numberStyle & fontStyle); break;
			case RDOSI_STRING       : theme->stringStyle     = (RDOFontStyle)(theme->stringStyle & fontStyle); break;
			case RDOSI_OPERATOR     : theme->operatorStyle   = (RDOFontStyle)(theme->operatorStyle & fontStyle); break;
		}
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFgColorCBChanged()
{
	COLORREF color = fgColorCB.getCurrentColor();
	fgColorCB.insertColor( color );
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;

	switch ( ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel() ) {
		case RDOSI_PLAINTEXT    : theme->defaultColor    = color; break;
		case RDOSI_IDENTIFICATOR: theme->identifierColor = color; break;
		case RDOSI_KEYWORD      : theme->keywordColor    = color; break;
		case RDOSI_FUNCTION     : theme->functionsColor  = color; break;
		case RDOSI_TRACE        : theme->traceColor      = color; break;
		case RDOSI_COMMENT      : theme->commentColor    = color; break;
		case RDOSI_NUMBER       : theme->numberColor     = color; break;
		case RDOSI_STRING       : theme->stringColor     = color; break;
		case RDOSI_OPERATOR     : theme->operatorColor   = color; break;
		case RDOSI_CARET        : theme->caretColor      = color; break;
		case RDOSI_BOOKMARK     : theme->bookmarkFgColor = color; break;
		case RDOSI_FOLD         : theme->foldFgColor     = color; break;
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnBgColorCBChanged()
{
	COLORREF color = bgColorCB.getCurrentColor();
	bgColorCB.insertColor( color );
	RDOEditorEditTheme* theme = &sheet->editorStyle.theme;

	switch ( ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel() ) {
		case RDOSI_PLAINTEXT:
		case RDOSI_IDENTIFICATOR:
		case RDOSI_KEYWORD:
		case RDOSI_FUNCTION:
		case RDOSI_TRACE:
		case RDOSI_COMMENT:
		case RDOSI_NUMBER:
		case RDOSI_STRING:
		case RDOSI_OPERATOR     : theme->backgroundColor  = color; break;
		case RDOSI_TEXTSELECTION: theme->selectionBgColor = color; break;
		case RDOSI_BOOKMARK     : theme->bookmarkBgColor  = color; break;
		case RDOSI_FOLD         : theme->foldBgColor      = color; break;
		case RDOSI_ERROR        : theme->errorBgColor     = color; break;
	}
	updateEdit();
}

void RDOEditorColorsOptions::OnFgColorBClick()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorCBChanged();
	}
}

void RDOEditorColorsOptions::OnBgColorBClick()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorCBChanged();
	}
}

void RDOEditorColorsOptions::OnBookmarkStyleCBChanged()
{
	CComboBox* bookmarkStyleCB = (CComboBox*)GetDlgItem( IDC_BOOKMARK_COMBO );
	if ( bookmarkStyleCB ) {
		sheet->editorStyle.theme.bookmarkStyle = (RDOBookmarkStyle)bookmarkStyleCB->GetCurSel();
		updateEdit();
	}
}

void RDOEditorColorsOptions::OnFoldStyleCBChanged()
{
	CComboBox* foldStyleCB = (CComboBox*)GetDlgItem( IDC_FOLD_COMBO );
	if ( foldStyleCB ) {
		sheet->editorStyle.theme.foldStyle = (RDOFoldStyle)foldStyleCB->GetCurSel();
		updateEdit();
	}
}

void RDOEditorColorsOptions::updateBookmarkStyle() const
{
	((CComboBox*)GetDlgItem( IDC_BOOKMARK_COMBO ))->SetCurSel( sheet->editorStyle.theme.bookmarkStyle );
}

void RDOEditorColorsOptions::updateFoldStyle() const
{
	((CComboBox*)GetDlgItem( IDC_FOLD_COMBO ))->SetCurSel( sheet->editorStyle.theme.foldStyle );
}

void RDOEditorColorsOptions::updateEdit()
{
	if ( edit ) {
		edit.setEditorStyle( sheet->editorStyle );
		SetModified( sheet->editorStyle.theme != sheet->prevStyle.theme || sheet->editorStyle.font != sheet->prevStyle.font );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorOptions, CPropertySheet )

	ON_WM_HELPINFO( )

END_MESSAGE_MAP()

RDOEditorOptions::RDOEditorOptions( const RDOEditorEditStyle& style ):
	CPropertySheet(),
	editorOptions( NULL ),
	colorOptions( NULL )
{
	CString s;
	s.LoadString( ID_OPTIONS );
	SetTitle( s );

	editorStyle = style;
	prevStyle   = style;

	editorOptions = new RDOEditorEditorOptions( *this, ((RDOEditorMainFrame*)AfxGetMainWnd())->getDebug(), rdoEditorProject.canClearBuffer, rdoEditorProject.clearBufferDelay );
	colorOptions  = new RDOEditorColorsOptions( *this );
	AddPage( editorOptions );
	AddPage( colorOptions );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOEditorOptions::~RDOEditorOptions()
{
	if ( editorOptions ) delete editorOptions;
	if ( colorOptions ) delete colorOptions;
}

void RDOEditorOptions::apply()
{
	RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
	int itemCount = tabCtrl->getItemCount();
	for ( int i = 0; i < itemCount; i++ ) {
		tabCtrl->getItemEdit( i )->setEditorStyle( editorStyle );
	}
	editorStyle.save();
	prevStyle = editorStyle;
	colorOptions->currentTheme = editorStyle.theme;
}

int CALLBACK RDOEditorOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam)
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

void RDOEditorOptions::onHelpButton()
{
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return;

	CPropertyPage* page = GetActivePage( );
	if ( page == editorOptions )
		filename += "::/html/work_options.htm#edit";
	if ( page == colorOptions )
		filename += "::/html/work_options.htm#color";
//	HtmlHelp( ::GetDesktopWindow(), filename, HH_DISPLAY_TOPIC, NULL );
}

BOOL RDOEditorOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return TRUE;

//	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
//		return HtmlHelp( ::GetDesktopWindow(), filename, HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
*/
