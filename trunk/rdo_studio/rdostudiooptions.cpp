#include "stdafx.h"
#include "rdostudiooptions.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;
using namespace rdoEditor;

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
BEGIN_MESSAGE_MAP(RDOStudioOptionsStylesAndColors, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioOptionsStylesAndColors)
	ON_NOTIFY(TVN_SELCHANGED, IDC_STYLEITEM_TREE, OnStyleItemChanged)
	ON_CBN_SELCHANGE(IDC_PREVIEWAS_COMBO, OnPreviewAsChanged)
	ON_CBN_SELCHANGE(IDC_FONTNAME_COMBO, OnFontNameChanged)
	ON_CBN_SELCHANGE(IDC_FONTSIZE_COMBO, OnFontSizeChanged)
	ON_BN_CLICKED(IDC_FONTBOLD_CHECK, OnFontStyleBoldChanged)
	ON_BN_CLICKED(IDC_FONTITALIC_CHECK, OnFontStyleItalicChanged)
	ON_BN_CLICKED(IDC_FONTUNDERLINE_CHECK, OnFontStyleUnderlineChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOptionsStylesAndColors::RDOStudioOptionsStylesAndColors( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	isCurrentFixed( false ),
	previewAs( STYLEObject::none ),
	all_font_name( "" ),
	all_font_size( -1 ),
	all_font_style( rdoEditCtrl::RDOFS_NONE ),
	null_font_style( rdoEditCtrl::RDOFS_NONE )
{
	//{{AFX_DATA_INIT(RDOStudioOptionsStylesAndColors)
	//}}AFX_DATA_INIT

	STYLEObject* object;
	object = new STYLEObject( STYLEObject::all, all_font_name, all_font_size );
	object->properties.push_back( new STYLEProperty( object, "All Windows", true, all_font_style ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::source, sheet->style_editor.font->name, sheet->style_editor.font->size );
	object->properties.push_back( new STYLEProperty( object, "Source Windows", true , static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->defaultStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_PLAINTEXT ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->defaultStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_IDENTIFICATOR ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->identifierStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_KEYWORD ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->keywordStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_FUNCTION ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->functionsStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_TRACE ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->traceStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_COMMENT ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->commentStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_NUMBER ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->numberStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_STRING ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->stringStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_OPERATOR ), false, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->operatorStyle ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_CARET ), false, null_font_style ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_TEXTSELECTION ), false, null_font_style ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_BOOKMARK ), false, null_font_style ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_FOLD ), false, null_font_style ) );
	object->properties.push_back( new STYLEProperty( object, format( ID_COLORSTYLE_EDITOR_ERROR ), false, null_font_style ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::build, sheet->style_build.font->name, sheet->style_build.font->size, false );
	object->properties.push_back( new STYLEProperty( object, "Build Windows", true, static_cast<RDOLogEditTheme*>(sheet->style_build.theme)->defaultStyle ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::debug, sheet->style_debug.font->name, sheet->style_debug.font->size, false );
	object->properties.push_back( new STYLEProperty( object, "Debug Windows", true, sheet->style_debug.theme->defaultStyle ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::tracer, sheet->style_editor.font->name, sheet->style_editor.font->size );
//	object = new STYLEObject( STYLEObject::tracer, sheet->style_tracer.font->name, sheet->style_tracer.font->size );
	object->properties.push_back( new STYLEProperty( object, "Trace Windows", true, static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme)->defaultStyle ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::results, sheet->style_results.font->name, sheet->style_results.font->size );
	object->properties.push_back( new STYLEProperty( object, "Results Windows", true, static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme)->defaultStyle ) );
	objects.push_back( object );

	object = new STYLEObject( STYLEObject::find, sheet->style_find.font->name, sheet->style_find.font->size );
	object->properties.push_back( new STYLEProperty( object, "Find Windows", true, static_cast<RDOFindEditTheme*>(sheet->style_find.theme)->defaultStyle ) );
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

	sheet->preview_editor.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->preview_editor.setEditorStyle( &sheet->style_editor );
	sheet->preview_editor.replaceCurrent( format( ID_OPTIONS_COLORS_EDITTEXT ), 0 );
	sheet->preview_editor.setReadOnly( true );
	sheet->preview_editor.bookmarkToggle();

	sheet->preview_build.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->preview_build.setEditorStyle( &sheet->style_build );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "Building Model..." ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "Wrong parameter value: L", rdoModelObjects::PAT, 40, true ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( "1 error(s) found." ) );
	sheet->preview_build.gotoNext();

	sheet->preview_debug.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->preview_debug.setEditorStyle( &sheet->style_debug );
	sheet->preview_debug.appendLine( "Start executing\r\n" );
	sheet->preview_debug.appendLine( "EI 0 1 1 0\r\n" );
	sheet->preview_debug.appendLine( "ES 0 1\r\n" );

	sheet->preview_tracer.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
//	sheet->preview_tracer.setEditorStyle( &sheet->style_tracer );

	sheet->preview_results.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
	sheet->preview_results.setEditorStyle( &sheet->style_results );
	sheet->preview_results.setReadOnly( false );
	sheet->preview_results.replaceCurrent( format( ID_OPTIONS_COLOR_RESULTSTEXT ), 0 );
	sheet->preview_results.setReadOnly( true );

	sheet->preview_find.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, -1 );
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
	sheet->preview_tracer.MoveWindow( rectEdit );
	sheet->preview_results.MoveWindow( rectEdit );
	sheet->preview_find.MoveWindow( rectEdit );

	list< STYLEObject* >::const_iterator obj_it = objects.begin();
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

	OnUpdateModify();

	m_styleItem.SelectItem( m_styleItem.GetRootItem() );

	return true;
}

void RDOStudioOptionsStylesAndColors::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

void RDOStudioOptionsStylesAndColors::OnStyleItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM item = m_styleItem.GetSelectedItem();
	if ( item ) {
		STYLEProperty* prop = reinterpret_cast<STYLEProperty*>(m_styleItem.GetItemData( item ));

		// Update font name combobox
		if ( prop->object->type == STYLEObject::all ) {
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
		m_fontStyleBold.SetCheck( prop->font_style & rdoEditCtrl::RDOFS_BOLD );
		m_fontStyleItalic.SetCheck( prop->font_style & rdoEditCtrl::RDOFS_ITALIC );
		m_fontStyleUnderline.SetCheck( prop->font_style & rdoEditCtrl::RDOFS_UNDERLINE );

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
			case STYLEObject::tracer: {
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
//				sheet->style_tracer.font->name  = all_font_name;
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
			case STYLEObject::tracer: {
//				sheet->style_tracer.font->name = str;
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
//				sheet->style_tracer.font->size  = all_font_size;
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
			case STYLEObject::tracer: {
//				sheet->style_tracer.font->size = size;
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
		prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style & ~rdoEditCtrl::RDOFS_BOLD);
		if ( m_fontStyleBold.GetCheck() ) {
			prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style | rdoEditCtrl::RDOFS_BOLD);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontStyleItalicChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style & ~rdoEditCtrl::RDOFS_ITALIC);
		if ( m_fontStyleItalic.GetCheck() ) {
			prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style | rdoEditCtrl::RDOFS_ITALIC);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsStylesAndColors::OnFontStyleUnderlineChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style & ~rdoEditCtrl::RDOFS_UNDERLINE);
		if ( m_fontStyleUnderline.GetCheck() ) {
			prop->font_style = static_cast<rdoEditCtrl::RDOFontStyle>(prop->font_style | rdoEditCtrl::RDOFS_UNDERLINE);
		}
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
			setPreviewAsCombo( STYLEObject::tracer );
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
//	             *sheet->style_tracer.font  != *studioApp.mainFrame->style_tracer.font ||
	             *sheet->style_results.font != *studioApp.mainFrame->style_results.font ||
	             *sheet->style_find.font    != *studioApp.mainFrame->style_find.font ||
	             *static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme) != *static_cast<RDOEditorEditTheme*>(studioApp.mainFrame->style_editor.theme) ||
	             *static_cast<RDOLogEditTheme*>(sheet->style_build.theme) != *static_cast<RDOLogEditTheme*>(studioApp.mainFrame->style_build.theme) ||
	             *sheet->style_debug.theme != *studioApp.mainFrame->style_debug.theme ||
//	             *static_cast<RDOTracerTheme*>(sheet->style_tracer.theme) != *static_cast<RDOTracerTheme*>(studioApp.mainFrame->style_tracer.theme)
	             *static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme) != *static_cast<RDOEditorBaseEditTheme*>(studioApp.mainFrame->style_results.theme) ||
	             *static_cast<RDOFindEditTheme*>(sheet->style_find.theme) != *static_cast<RDOFindEditTheme*>(studioApp.mainFrame->style_find.theme) );
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

const RDOStudioOptionsStylesAndColors::STYLEObject* RDOStudioOptionsStylesAndColors::getCurrentObject() const
{
	STYLEProperty* prop = getCurrentProperty();
	return prop ? prop->object : NULL;
}

RDOStudioOptionsStylesAndColors::STYLEObject::Type RDOStudioOptionsStylesAndColors::getCurrentObjectType() const
{
	const STYLEObject* obj = getCurrentObject();
	return obj ? obj->type : STYLEObject::none;
}

void RDOStudioOptionsStylesAndColors::setPreviewAsCombo( STYLEObject::Type type )
{
	if ( previewAs != type && type >= STYLEObject::source ) {
		previewAs = type;
		sheet->preview_editor.ShowWindow( SW_HIDE );
		sheet->preview_build.ShowWindow( SW_HIDE );
		sheet->preview_debug.ShowWindow( SW_HIDE );
		sheet->preview_tracer.ShowWindow( SW_HIDE );
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
			case STYLEObject::tracer: {
				m_previewAs.SetCurSel( 3 );
				sheet->preview_tracer.ShowWindow( SW_SHOW );
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

void RDOStudioOptionsStylesAndColors::updatePropOfAllObject() const
{
	list< STYLEObject* >::const_iterator it_first = objects.begin();
	if ( it_first != objects.end() ) {
		list< STYLEObject* >::const_iterator it = it_first;
		it++;
		if ( it != objects.end() ) {
			string font_name = (*it)->font_name;
			int    font_size = (*it)->font_size;
			it++;
			bool flag_font_name = true;
			bool flag_font_size = true;
			while ( it != objects.end() ) {
				if ( (*it)->font_name != font_name ) {
					flag_font_name = false;
				}
				if ( (*it)->font_size != font_size ) {
					flag_font_size = false;
				}
				if ( !flag_font_name && !flag_font_size ) break;
				it++;
			}
			(*it_first)->font_name = flag_font_name ? font_name : "";
			(*it_first)->font_size = flag_font_size ? font_size : -1;
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
//	style_tracer.init();
	style_results.init();
	style_find.init();

	style_editor  = studioApp.mainFrame->style_editor;
	style_build   = studioApp.mainFrame->style_build;
	style_debug   = studioApp.mainFrame->style_debug;
//	style_tracer  = studioApp.mainFrame->style_tracer;
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
//	if ( preview_tracer.GetSafeHwnd() ) {
//		preview_tracer.setEditorStyle( &style_tracer );
//	}
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
//	studioApp.mainFrame->style_tracer  = style_tracer;
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
