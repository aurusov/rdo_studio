#include "stdafx.h"
#include "rdostudiooptions.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoEditCtrl;
using namespace rdoTracerLog;

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
	ON_BN_CLICKED(IDC_MARGIN_FOLD_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARGIN_BOOKMARK_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARGIN_LINENUMBER_CHECK, OnUpdateModify)
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
	m_marginFold = FALSE;
	m_marginBookmark = FALSE;
	m_marginLineNumber = FALSE;
	//}}AFX_DATA_INIT

	m_bufferClearAuto = sheet->style_editor.buffer->canClearBuffer ? 1 : 0;
	m_bufferDelay     = sheet->style_editor.buffer->clearBufferDelay;

	m_codecompUse          = sheet->style_editor.autoComplete->useAutoComplete ? 1 : 0;
	m_codecompShowFullList = sheet->style_editor.autoComplete->showFullList ? 0 : 1;

	m_marginFold       = sheet->style_editor.margin->fold ? 1 : 0;
	m_marginBookmark   = sheet->style_editor.margin->bookmark ? 1 : 0;
	m_marginLineNumber = sheet->style_editor.margin->lineNumber ? 1 : 0;
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
	DDX_Check(pDX, IDC_MARGIN_FOLD_CHECK, m_marginFold);
	DDX_Check(pDX, IDC_MARGIN_BOOKMARK_CHECK, m_marginBookmark);
	DDX_Check(pDX, IDC_MARGIN_LINENUMBER_CHECK, m_marginLineNumber);
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

	sheet->style_editor.margin->fold       = m_marginFold ? true : false;
	sheet->style_editor.margin->bookmark   = m_marginBookmark ? true : false;
	sheet->style_editor.margin->lineNumber = m_marginLineNumber ? true : false;

	if ( sheet->preview_editor.GetSafeHwnd() ) {
		sheet->preview_editor.setEditorStyle( &sheet->style_editor );
	}

	SetModified( *sheet->style_editor.buffer != *studioApp.mainFrame->style_editor.buffer || *sheet->style_editor.autoComplete != *studioApp.mainFrame->style_editor.autoComplete || *sheet->style_editor.margin != *studioApp.mainFrame->style_editor.margin );
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

	*sheet->style_build.tab   = *sheet->style_editor.tab;
	*sheet->style_debug.tab   = *sheet->style_editor.tab;
	*sheet->style_results.tab = *sheet->style_editor.tab;
	*sheet->style_find.tab    = *sheet->style_editor.tab;

	sheet->updateStyles();

	SetModified( *sheet->style_editor.tab != *studioApp.mainFrame->style_editor.tab );
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsStylesAndColors
// ----------------------------------------------------------------------------
COLORREF RDOStudioOptionsStylesAndColors::null_fg_color = RGB( 0x00, 0x00, 0x00 );
COLORREF RDOStudioOptionsStylesAndColors::null_bg_color = RGB( 0xFF, 0xFF, 0xFF );
bool RDOStudioOptionsStylesAndColors::null_wordwrap      = false;
bool RDOStudioOptionsStylesAndColors::null_horzscrollbar = true;

BEGIN_MESSAGE_MAP(RDOStudioOptionsStylesAndColors, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsStylesAndColors)
	ON_NOTIFY(TVN_SELCHANGED, IDC_STYLEITEM_TREE, OnStyleItemChanged)
	ON_CBN_SELCHANGE(IDC_PREVIEWAS_COMBO, OnPreviewAsChanged)
	ON_CBN_SELCHANGE(IDC_FONTNAME_COMBO, OnFontNameChanged)
	ON_CBN_SELCHANGE(IDC_FONTSIZE_COMBO, OnFontSizeChanged)
	ON_BN_CLICKED(IDC_FONTBOLD_CHECK, OnFontStyleBoldChanged)
	ON_BN_CLICKED(IDC_FONTITALIC_CHECK, OnFontStyleItalicChanged)
	ON_BN_CLICKED(IDC_FONTUNDERLINE_CHECK, OnFontStyleUnderlineChanged)
	ON_CBN_SELCHANGE(IDC_FGCOLOR_COMBO, OnFgColorChanged)
	ON_CBN_SELCHANGE(IDC_BGCOLOR_COMBO, OnBgColorChanged)
	ON_BN_CLICKED(IDC_FGCOLOR_BUTTON, OnFgColorClicked)
	ON_BN_CLICKED(IDC_BGCOLOR_BUTTON, OnBgColorClicked)
	ON_BN_CLICKED(IDC_WORDWRAP_CHECK, OnWordWrapClicked)
	ON_BN_CLICKED(IDC_HORZSCROLLBAR_CHECK, OnHorzScrollBarClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsStylesAndColors::RDOStudioOptionsStylesAndColors( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	isCurrentFixed( false ),
	previewAs( STYLEObject::none ),
	all_font_name( "" ),
	all_font_size( -1 ),
	all_fg_color( RGB( 0x00, 0x00, 0x00 ) ),
	all_bg_color( RGB( 0xFF, 0xFF, 0xFF ) ),
	use_all_fg_color( false ),
	use_all_bg_color( false ),
	null_font_style( RDOFS_NONE )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsStylesAndColors)
	//}}AFX_DATA_INIT

	STYLEObject* object;
	object = new STYLEObject( STYLEObject::all, all_font_name, all_font_size );
	object->properties.push_back( new STYLEProperty( object, "All Windows", null_font_style, all_fg_color, all_bg_color ) );
	objects.push_back( object );

	RDOEditorEditTheme* editor_theme = static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme);
	object = new STYLEObject( STYLEObject::source, sheet->style_editor.font->name, sheet->style_editor.font->size, true, sheet->style_editor.window->wordWrap, sheet->style_editor.window->showHorzScrollBar );
	object->properties.push_back( new STYLEProperty( object, "Source Windows", editor_theme->defaultStyle, editor_theme->identifierColor, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_PLAINTEXT ), editor_theme->defaultStyle, editor_theme->defaultColor, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_IDENTIFICATOR ), editor_theme->identifierStyle, editor_theme->identifierColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_KEYWORD ), editor_theme->keywordStyle, editor_theme->keywordColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_FUNCTION ), editor_theme->functionsStyle, editor_theme->keywordColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_TRACE ), editor_theme->traceStyle, editor_theme->traceColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_COMMENT ), editor_theme->commentStyle, editor_theme->commentColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_NUMBER ), editor_theme->numberStyle, editor_theme->numberColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_STRING ), editor_theme->stringStyle, editor_theme->stringColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_OPERATOR ), editor_theme->operatorStyle, editor_theme->operatorColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_CARET ), null_font_style, editor_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, editor_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, editor_theme->bookmarkFgColor, editor_theme->bookmarkBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_FOLD ), null_font_style, editor_theme->foldFgColor, editor_theme->foldBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_ERROR ), null_font_style, null_fg_color, editor_theme->errorBgColor ) );
	objects.push_back( object );

	RDOLogEditTheme* build_theme = static_cast<RDOLogEditTheme*>(sheet->style_build.theme);
	object = new STYLEObject( STYLEObject::build, sheet->style_build.font->name, sheet->style_build.font->size, false, sheet->style_build.window->wordWrap, sheet->style_build.window->showHorzScrollBar );
	object->properties.push_back( new STYLEProperty( object, "Build Window", build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, "text", build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, "selected line", null_font_style, null_fg_color, build_theme->selectLineBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_CARET ), null_font_style, build_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, build_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, build_theme->bookmarkFgColor, build_theme->bookmarkBgColor ) );
	objects.push_back( object );

	RDOBaseEditTheme* debug_theme = sheet->style_debug.theme;
	object = new STYLEObject( STYLEObject::debug, sheet->style_debug.font->name, sheet->style_debug.font->size, false );
	object->properties.push_back( new STYLEProperty( object, "Debug Window", debug_theme->defaultStyle, debug_theme->defaultColor, debug_theme->backgroundColor ) );
	objects.push_back( object );

	RDOTracerLogStyle* trace_theme = &sheet->style_trace;
	object = new STYLEObject( STYLEObject::trace, sheet->style_trace.font->name, sheet->style_trace.font->size );
	object->properties.push_back( new STYLEProperty( object, "Trace Window", trace_theme->style, trace_theme->es.foregroundColor, trace_theme->es.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, "Service event (ES)", trace_theme->style, trace_theme->es.foregroundColor, trace_theme->es.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, "Action start (EB)", trace_theme->style, trace_theme->eb.foregroundColor, trace_theme->eb.backgroundColor ) );
	objects.push_back( object );

	RDOEditorBaseEditTheme* results_theme = static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme);
	object = new STYLEObject( STYLEObject::results, sheet->style_results.font->name, sheet->style_results.font->size );
	object->properties.push_back( new STYLEProperty( object, "Results Window", results_theme->defaultStyle, results_theme->defaultColor, results_theme->backgroundColor ) );
	objects.push_back( object );

	RDOFindEditTheme* find_theme = static_cast<RDOFindEditTheme*>(sheet->style_find.theme);
	object = new STYLEObject( STYLEObject::find, sheet->style_find.font->name, sheet->style_find.font->size );
	object->properties.push_back( new STYLEProperty( object, "Find Window", find_theme->defaultStyle, find_theme->defaultColor, find_theme->backgroundColor ) );
	objects.push_back( object );
}

RDOStudioOptionsStylesAndColors::~RDOStudioOptionsStylesAndColors()
{
	list< STYLEObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		delete *it++;
	};
}

void RDOStudioOptionsStylesAndColors::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioOptionsStylesAndColors)
	DDX_Control(pDX, IDC_HORZSCROLLBAR_CHECK, m_horzScrollBar);
	DDX_Control(pDX, IDC_WORDWRAP_CHECK, m_wordWrap);
	DDX_Control(pDX, IDC_FOLD_COMBO, m_fold);
	DDX_Control(pDX, IDC_FOLD_STATIC, m_foldStatic);
	DDX_Control(pDX, IDC_BOOKMARK_STATIC, m_bookmarkStatic);
	DDX_Control(pDX, IDC_BOOKMARK_COMBO, m_bookmark);
	DDX_Control(pDX, IDC_PREVIEWAS_STATIC, m_previewAsStatic);
	DDX_Control(pDX, IDC_BGCOLOR_STATIC, m_bgColorStatic);
	DDX_Control(pDX, IDC_FGCOLOR_STATIC, m_fgColorStatic);
	DDX_Control(pDX, IDC_BGCOLOR_BUTTON, m_bgColorButton);
	DDX_Control(pDX, IDC_FGCOLOR_BUTTON, m_fgColorButton);
	DDX_Control(pDX, IDC_FONTUNDERLINE_CHECK, m_fontStyleUnderline);
	DDX_Control(pDX, IDC_FONTITALIC_CHECK, m_fontStyleItalic);
	DDX_Control(pDX, IDC_FONTBOLD_CHECK, m_fontStyleBold);
	DDX_Control(pDX, IDC_FONTSIZE_COMBO, m_fontSize);
	DDX_Control(pDX, IDC_PREVIEWAS_COMBO, m_previewAs);
	DDX_Control(pDX, IDC_FONTNAME_COMBO, m_fontName);
	DDX_Control(pDX, IDC_STYLEITEM_TREE, m_styleItem);
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}

int CALLBACK RDOStudioOptionsStylesAndColors::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam )
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

	sheet->preview_editor.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_editor.setEditorStyle( &sheet->style_editor );
	sheet->preview_editor.appendText( format( ID_OPTIONS_COLORS_EDITTEXT ) );
	sheet->preview_editor.scrollToLine( 0 );
	sheet->preview_editor.setReadOnly( true );
	sheet->preview_editor.bookmarkToggle();
	sheet->preview_editor.setErrorLine( sheet->preview_editor.getLineCount() - 1 );

	sheet->preview_build.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_build.setEditorStyle( &sheet->style_build );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "Building Model..." ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "Wrong parameter value: L", rdoModelObjects::PAT, 40, true ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "1 error(s) found." ) );
	sheet->preview_build.gotoNext();

	sheet->preview_debug.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_debug.setEditorStyle( &sheet->style_debug );
	sheet->preview_debug.appendLine( "Start executing\r\n" );
	sheet->preview_debug.appendLine( "EI 0 1 1 0\r\n" );
	sheet->preview_debug.appendLine( "ES 0 1\r\n" );

	sheet->preview_trace.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_trace.setStyle( &sheet->style_trace );

	sheet->preview_results.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_results.setEditorStyle( &sheet->style_results );
	sheet->preview_results.setReadOnly( false );
	sheet->preview_results.replaceCurrent( format( ID_OPTIONS_COLOR_RESULTSTEXT ), 0 );
	sheet->preview_results.setReadOnly( true );

	sheet->preview_find.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_find.setEditorStyle( &sheet->style_find );
	sheet->preview_find.setKeyword( "$Time" );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( "Searching for '$Time'..." ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( "$Time = Равномерный(0.25, 0.75)", rdoModelObjects::PAT, 3 ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( "$Time = Нормальный(0.45, 0.2)", rdoModelObjects::PAT, 13 ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( "'2' occurrence(s) have been found." ) );
	sheet->preview_find.gotoNext();

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

	sheet->preview_editor.MoveWindow( rectEdit );
	sheet->preview_build.MoveWindow( rectEdit );
	sheet->preview_debug.MoveWindow( rectEdit );
	sheet->preview_trace.MoveWindow( rectEdit );
	sheet->preview_results.MoveWindow( rectEdit );
	sheet->preview_find.MoveWindow( rectEdit );

	list< STYLEObject* >::const_iterator obj_it = objects.begin();
	while ( obj_it != objects.end() ) {
		list< STYLEProperty* >::iterator prop_it = (*obj_it)->properties.begin();
		HTREEITEM root = TVI_ROOT;
		while ( prop_it != (*obj_it)->properties.end() ) {
			if ( prop_it == (*obj_it)->properties.begin() ) {
				root = TVI_ROOT;
			}
			HTREEITEM item = m_styleItem.InsertItem( (*prop_it)->name.c_str(), root );
			m_styleItem.SetItemData( item, reinterpret_cast<DWORD>(*prop_it) );
			if ( prop_it == (*obj_it)->properties.begin() ) {
				root = item;
			}
			prop_it++;
		}
		obj_it++;
	}

	LOGFONT lf;
	lf.lfCharSet = static_cast<unsigned char>(sheet->style_editor.font->characterSet);
	lf.lfFaceName[0] = '\0';
	::EnumFontFamiliesEx( GetDC()->m_hDC, &lf, reinterpret_cast<FONTENUMPROC>(RDOStudioOptionsStylesAndColors::EnumFontFamExProc), reinterpret_cast<LPARAM>(&fonts), 0 );
	isCurrentFixed = false;
	loadFontsIntoCombo( true );

	setPreviewAsCombo( STYLEObject::source );

	updatePropOfAllObject();
	m_styleItem.SelectItem( m_styleItem.GetRootItem() );

	return true;
}

void RDOStudioOptionsStylesAndColors::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsStylesAndColors::OnStyleItemChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	HTREEITEM item = m_styleItem.GetSelectedItem();
	if ( item ) {
		STYLEProperty* prop = reinterpret_cast<STYLEProperty*>(m_styleItem.GetItemData( item ));
		STYLEObject::Type type = prop->object->type;

		// Update font name combobox
		if ( type == STYLEObject::all ) {
			loadFontsIntoCombo( true );
		} else {
			loadFontsIntoCombo( prop->object->font_fixed );
		}
		list< STYLEFont >::const_iterator it = fonts.begin();
		bool flag = false;
		while ( it != fonts.end() ) {
			if ( it->name == prop->object->font_name ) {
				int index = m_fontName.FindStringExact( -1, prop->object->font_name.c_str() );
				if ( index != CB_ERR ) {
					m_fontName.SetCurSel( index );
					flag = true;
				}
				break;
			}
			it++;
		}
		if ( !flag ) {
			m_fontName.SetCurSel( -1 );
		}

		// Update font size combobox
		int index = m_fontSize.FindStringExact( -1, format( "%d", prop->object->font_size ).c_str() );
		if ( index != CB_ERR ) {
			m_fontSize.SetCurSel( index );
		} else {
			m_fontSize.SetCurSel( -1 );
		}

		// Update font style checkboxs
		bool flag_font_style = &prop->font_style != &null_font_style;
		m_fontStyleBold.EnableWindow( flag_font_style );
		m_fontStyleItalic.EnableWindow( flag_font_style );
		m_fontStyleUnderline.EnableWindow( flag_font_style );
		m_fontStyleBold.SetCheck( prop->font_style & RDOFS_BOLD );
		m_fontStyleItalic.SetCheck( prop->font_style & RDOFS_ITALIC );
		m_fontStyleUnderline.SetCheck( prop->font_style & RDOFS_UNDERLINE );

		// Update FG color combobox
		bool flag_fg_color = &prop->fg_color != &null_fg_color;
		fgColorCB.EnableWindow( flag_fg_color );
		m_fgColorStatic.EnableWindow( flag_fg_color );
		m_fgColorButton.EnableWindow( flag_fg_color );
		if ( flag_fg_color ) {
			fgColorCB.insertColor( prop->fg_color );
			fgColorCB.setCurrentColor( prop->fg_color );
		} else if ( &prop->fg_disable_color != &null_fg_color ) {
			fgColorCB.insertColor( prop->fg_disable_color );
			fgColorCB.setCurrentColor( prop->fg_disable_color );
		} else {
			fgColorCB.SetCurSel( -1 );
		}

		// Update BG color combobox
		bool flag_bg_color = &prop->bg_color != &null_bg_color;
		bgColorCB.EnableWindow( flag_bg_color );
		m_bgColorStatic.EnableWindow( flag_bg_color );
		m_bgColorButton.EnableWindow( flag_bg_color );
		if ( flag_bg_color ) {
			bgColorCB.insertColor( prop->bg_color );
			bgColorCB.setCurrentColor( prop->bg_color );
		} else if ( &prop->bg_disable_color != &null_bg_color ) {
			bgColorCB.insertColor( prop->bg_disable_color );
			bgColorCB.setCurrentColor( prop->bg_disable_color );
		} else {
			bgColorCB.SetCurSel( -1 );
		}

		// Update wordwrap
		bool flag_wordwrap = type == STYLEObject::source || type == STYLEObject::build || type == STYLEObject::debug || type == STYLEObject::results || type == STYLEObject::find;
		m_wordWrap.ShowWindow( flag_wordwrap ? SW_SHOW : SW_HIDE );
		m_horzScrollBar.ShowWindow( flag_wordwrap ? SW_SHOW : SW_HIDE );
		m_wordWrap.SetCheck( prop->object->wordwrap ? 1 : 0 );
		m_horzScrollBar.SetCheck( prop->object->horzscrollbar ? 1 : 0 );
		OnWordWrapClicked();

		// Update bookmark
		bool flag_bookmark = type == STYLEObject::source || type == STYLEObject::build || type == STYLEObject::debug || type == STYLEObject::results || type == STYLEObject::find;
		m_bookmark.ShowWindow( flag_bookmark ? SW_SHOW : SW_HIDE );
		m_bookmarkStatic.ShowWindow( flag_bookmark ? SW_SHOW : SW_HIDE );

		// Update fold
		bool flag_fold = type == STYLEObject::source;
		m_fold.ShowWindow( flag_fold ? SW_SHOW : SW_HIDE );
		m_foldStatic.ShowWindow( flag_fold ? SW_SHOW : SW_HIDE );

		// Update preview
		bool flag_preview = type == STYLEObject::all;
		m_previewAs.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		m_previewAsStatic.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		setPreviewAsCombo( type );

		switch ( type ) {
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
	}

	*pResult = 0;
}

void RDOStudioOptionsStylesAndColors::OnFontNameChanged()
{
	int index = m_fontName.GetCurSel();
	if ( index != CB_ERR ) {
		CString str;
		m_fontName.GetLBText( index, str );
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				all_font_name = str;
				sheet->style_editor.font->name  = all_font_name;
				sheet->style_build.font->name   = all_font_name;
				sheet->style_debug.font->name   = all_font_name;
				sheet->style_trace.font->name   = all_font_name;
				sheet->style_results.font->name = all_font_name;
				sheet->style_find.font->name    = all_font_name;
				break;
			}
			case STYLEObject::source: {
				sheet->style_editor.font->name = str;
				break;
			}
			case STYLEObject::build: {
				sheet->style_build.font->name = str;
				break;
			}
			case STYLEObject::debug: {
				sheet->style_debug.font->name = str;
				break;
			}
			case STYLEObject::trace: {
				sheet->style_trace.font->name = str;
				break;
			}
			case STYLEObject::results: {
				sheet->style_results.font->name = str;
				break;
			}
			case STYLEObject::find: {
				sheet->style_find.font->name = str;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontSizeChanged()
{
	int index = m_fontSize.GetCurSel();
	if ( index != CB_ERR ) {
		CString str;
		m_fontSize.GetLBText( index, str );
		int size = atoi( str );
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				all_font_size = size;
				sheet->style_editor.font->size  = all_font_size;
				sheet->style_build.font->size   = all_font_size;
				sheet->style_debug.font->size   = all_font_size;
				sheet->style_trace.font->size   = all_font_size;
				sheet->style_results.font->size = all_font_size;
				sheet->style_find.font->size    = all_font_size;
				break;
			}
			case STYLEObject::source: {
				sheet->style_editor.font->size = size;
				break;
			}
			case STYLEObject::build: {
				sheet->style_build.font->size = size;
				break;
			}
			case STYLEObject::debug: {
				sheet->style_debug.font->size = size;
				break;
			}
			case STYLEObject::trace: {
				sheet->style_trace.font->size = size;
				break;
			}
			case STYLEObject::results: {
				sheet->style_results.font->size = size;
				break;
			}
			case STYLEObject::find: {
				sheet->style_find.font->size = size;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontStyleBoldChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOFontStyle>(prop->font_style & ~RDOFS_BOLD);
		if ( m_fontStyleBold.GetCheck() ) {
			prop->font_style = static_cast<RDOFontStyle>(prop->font_style | RDOFS_BOLD);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontStyleItalicChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOFontStyle>(prop->font_style & ~RDOFS_ITALIC);
		if ( m_fontStyleItalic.GetCheck() ) {
			prop->font_style = static_cast<RDOFontStyle>(prop->font_style | RDOFS_ITALIC);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontStyleUnderlineChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOFontStyle>(prop->font_style & ~RDOFS_UNDERLINE);
		if ( m_fontStyleUnderline.GetCheck() ) {
			prop->font_style = static_cast<RDOFontStyle>(prop->font_style | RDOFS_UNDERLINE);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFgColorChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( &prop->fg_color != &null_fg_color ) {
		prop->fg_color = fgColorCB.getCurrentColor();
		if ( &prop->fg_color == &all_fg_color ) {
			list< STYLEObject* >::const_iterator it = objects.begin();
			if ( it != objects.end() ) {
				it++;
				while ( it != objects.end() ) {
					list< STYLEProperty* >::const_iterator it_prop = (*it)->properties.begin();
					if ( it_prop != (*it)->properties.end() ) {
						(*it_prop)->fg_color = all_fg_color;
					}
					it++;
				}
			}
		}
		fgColorCB.insertColor( prop->fg_color );
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnBgColorChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( &prop->bg_color != &null_bg_color ) {
		prop->bg_color = bgColorCB.getCurrentColor();
		if ( &prop->bg_color == &all_bg_color ) {
			list< STYLEObject* >::const_iterator it = objects.begin();
			if ( it != objects.end() ) {
				it++;
				while ( it != objects.end() ) {
					list< STYLEProperty* >::const_iterator it_prop = (*it)->properties.begin();
					if ( it_prop != (*it)->properties.end() ) {
						(*it_prop)->bg_color = all_bg_color;
					}
					it++;
				}
			}
		}
		bgColorCB.insertColor( prop->bg_color );
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFgColorClicked()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorChanged();
	}
}

void RDOStudioOptionsStylesAndColors::OnBgColorClicked()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorChanged();
	}
}

void RDOStudioOptionsStylesAndColors::OnWordWrapClicked()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->wordwrap != &null_wordwrap ) {
		obj->wordwrap = m_wordWrap.GetCheck() ? true : false;
		m_horzScrollBar.EnableWindow( !obj->wordwrap );
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnHorzScrollBarClicked()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->horzscrollbar != &null_horzscrollbar ) {
		obj->horzscrollbar = m_horzScrollBar.GetCheck() ? true : false;
		OnUpdateModify();
	}
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

void RDOStudioOptionsStylesAndColors::OnUpdateModify()
{
	UpdateData();

	updatePropOfAllObject();

	sheet->updateStyles();

	SetModified( *sheet->style_editor.font  != *studioApp.mainFrame->style_editor.font ||
	             *sheet->style_build.font   != *studioApp.mainFrame->style_build.font ||
	             *sheet->style_debug.font   != *studioApp.mainFrame->style_debug.font ||
	             *sheet->style_trace.font   != *studioApp.mainFrame->style_trace.font ||
	             *sheet->style_results.font != *studioApp.mainFrame->style_results.font ||
	             *sheet->style_find.font    != *studioApp.mainFrame->style_find.font ||
	             *static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme) != *static_cast<RDOEditorEditTheme*>(studioApp.mainFrame->style_editor.theme) ||
	             *static_cast<RDOLogEditTheme*>(sheet->style_build.theme) != *static_cast<RDOLogEditTheme*>(studioApp.mainFrame->style_build.theme) ||
	             *sheet->style_debug.theme != *studioApp.mainFrame->style_debug.theme ||
	             sheet->style_trace != studioApp.mainFrame->style_trace ||
	             *static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme) != *static_cast<RDOEditorBaseEditTheme*>(studioApp.mainFrame->style_results.theme) ||
	             *static_cast<RDOFindEditTheme*>(sheet->style_find.theme) != *static_cast<RDOFindEditTheme*>(studioApp.mainFrame->style_find.theme) ||
	             *sheet->style_editor.window  != *studioApp.mainFrame->style_editor.window ||
	             *sheet->style_build.window   != *studioApp.mainFrame->style_build.window ||
	             *sheet->style_debug.window   != *studioApp.mainFrame->style_debug.window ||
	             *sheet->style_results.window != *studioApp.mainFrame->style_results.window ||
	             *sheet->style_find.window    != *studioApp.mainFrame->style_find.window );
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

RDOStudioOptionsStylesAndColors::STYLEProperty* RDOStudioOptionsStylesAndColors::getCurrentProperty() const
{
	HTREEITEM item = m_styleItem.GetSelectedItem();
	return item ? reinterpret_cast<RDOStudioOptionsStylesAndColors::STYLEProperty*>(m_styleItem.GetItemData( item )) : NULL;
}

RDOStudioOptionsStylesAndColors::STYLEObject* RDOStudioOptionsStylesAndColors::getCurrentObject() const
{
	STYLEProperty* prop = getCurrentProperty();
	return prop ? prop->object : NULL;
}

RDOStudioOptionsStylesAndColors::STYLEObject::Type RDOStudioOptionsStylesAndColors::getCurrentObjectType() const
{
	STYLEObject* obj = getCurrentObject();
	return obj ? obj->type : STYLEObject::none;
}

void RDOStudioOptionsStylesAndColors::setPreviewAsCombo( STYLEObject::Type type )
{
	if ( previewAs != type && type >= STYLEObject::source ) {
		previewAs = type;
		sheet->preview_editor.ShowWindow( SW_HIDE );
		sheet->preview_build.ShowWindow( SW_HIDE );
		sheet->preview_debug.ShowWindow( SW_HIDE );
		sheet->preview_trace.ShowWindow( SW_HIDE );
		sheet->preview_results.ShowWindow( SW_HIDE );
		sheet->preview_find.ShowWindow( SW_HIDE );
		switch ( previewAs ) {
			case STYLEObject::source: {
				m_previewAs.SetCurSel( 0 );
				sheet->preview_editor.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::build: {
				m_previewAs.SetCurSel( 1 );
				sheet->preview_build.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::debug: {
				m_previewAs.SetCurSel( 2 );
				sheet->preview_debug.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::trace: {
				m_previewAs.SetCurSel( 3 );
				sheet->preview_trace.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::results: {
				m_previewAs.SetCurSel( 4 );
				sheet->preview_results.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::find: {
				m_previewAs.SetCurSel( 5 );
				sheet->preview_find.ShowWindow( SW_SHOW );
				break;
			}
			default: break;
		}
	}
}

void RDOStudioOptionsStylesAndColors::updatePropOfAllObject()
{
	list< STYLEObject* >::const_iterator it = objects.begin();
	if ( it != objects.end() ) {
		it++;
		if ( it != objects.end() ) {
			all_font_name  = (*it)->font_name;
			all_font_size  = (*it)->font_size;
			bool use_color = false;
			list< STYLEProperty* >::const_iterator prop = (*it)->properties.begin();
			if ( prop != (*it)->properties.end() ) {
				all_fg_color = (*prop)->fg_color;
				all_bg_color = (*prop)->bg_color;
				use_color    = true;
			}
			it++;
			bool flag_font_name = true;
			bool flag_font_size = true;
			bool flag_fg_clor   = true;
			bool flag_bg_clor   = true;
			while ( it != objects.end() ) {
				if ( flag_font_name && (*it)->font_name != all_font_name ) {
					flag_font_name = false;
					all_font_name  = "";
				}
				if ( flag_font_size && (*it)->font_size != all_font_size ) {
					flag_font_size = false;
					all_font_size  = -1;
				}
				prop = (*it)->properties.begin();
				if ( use_color && prop != (*it)->properties.end() ) {
					if ( flag_fg_clor && (*prop)->fg_color != all_fg_color ) {
						flag_fg_clor = false;
					}
					if ( flag_bg_clor && (*prop)->bg_color != all_bg_color ) {
						flag_bg_clor = false;
					}
				}
				if ( !flag_font_name && !flag_font_size && !flag_fg_clor && !flag_bg_clor ) break;
				it++;
			}
			use_all_fg_color = use_color && flag_fg_clor;
			use_all_bg_color = use_color && flag_bg_clor;
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
	style_trace.init();
	style_results.init();
	style_find.init();

	style_editor  = studioApp.mainFrame->style_editor;
	style_build   = studioApp.mainFrame->style_build;
	style_debug   = studioApp.mainFrame->style_debug;
	style_trace   = studioApp.mainFrame->style_trace;
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

void RDOStudioOptions::updateStyles()
{
	if ( preview_editor.GetSafeHwnd() ) {
		preview_editor.setEditorStyle( &style_editor );
	}
	if ( preview_build.GetSafeHwnd() ) {
		preview_build.setEditorStyle( &style_build );
	}
	if ( preview_debug.GetSafeHwnd() ) {
		preview_debug.setEditorStyle( &style_debug );
	}
	if ( preview_trace.GetSafeHwnd() ) {
		preview_trace.setStyle( &style_trace );
	}
	if ( preview_results.GetSafeHwnd() ) {
		preview_results.setEditorStyle( &style_results );
	}
	if ( preview_find.GetSafeHwnd() ) {
		preview_find.setEditorStyle( &style_find );
	}
}

void RDOStudioOptions::apply() const
{
	studioApp.mainFrame->style_editor  = style_editor;
	studioApp.mainFrame->style_build   = style_build;
	studioApp.mainFrame->style_debug   = style_debug;
	studioApp.mainFrame->style_trace   = style_trace;
	studioApp.mainFrame->style_results = style_results;
	studioApp.mainFrame->style_find    = style_find;
	studioApp.mainFrame->updateAllStyles();
}

int CALLBACK RDOStudioOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM /*lParam*/)
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
