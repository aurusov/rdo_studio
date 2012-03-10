/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/options.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/options.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/plugins.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoStyle;
using namespace rdoEditCtrl;
using namespace rdoTracerLog;

#define timerPlugin_ID 1

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsGeneral
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsGeneral, CPropertyPage)
	ON_BN_CLICKED(IDC_FILEASSOCIATION_SETUP_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_FILEASSOCIATION_CHECKINFUTURE_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_OPENLASTPROJECT_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_SHOWCAPTIONFULLNAME_CHECK, OnUpdateModify)
END_MESSAGE_MAP()

RDOStudioOptionsGeneral::RDOStudioOptionsGeneral( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	m_setup = FALSE;
	m_checkInFuture = FALSE;
	m_openLastProject = FALSE;
	m_showFullName = FALSE;

	m_setup           = studioApp.getFileAssociationSetup();
	m_checkInFuture   = studioApp.getFileAssociationCheckInFuture();
	m_openLastProject = studioApp.getOpenLastProject();
	m_showFullName    = studioApp.getShowCaptionFullName();

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioOptionsGeneral::~RDOStudioOptionsGeneral()
{
}

void RDOStudioOptionsGeneral::DoDataExchange(CDataExchange* pDX) 
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FILEASSOCIATION_SETUP_CHECK, m_setup);
	DDX_Check(pDX, IDC_FILEASSOCIATION_CHECKINFUTURE_CHECK, m_checkInFuture);
	DDX_Check(pDX, IDC_OPENLASTPROJECT_CHECK, m_openLastProject);
	DDX_Check(pDX, IDC_SHOWCAPTIONFULLNAME_CHECK, m_showFullName);
}

void RDOStudioOptionsGeneral::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioOptionsGeneral::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioOptionsGeneral::OnUpdateModify() 
{
	UpdateData();
	SetModified( m_setup != (studioApp.getFileAssociationSetup() ? 1 : 0) || m_checkInFuture != (studioApp.getFileAssociationCheckInFuture() ? 1 : 0) || m_openLastProject != (studioApp.getOpenLastProject() ? 1 : 0) || m_showFullName != (studioApp.getShowCaptionFullName() ? 1 : 0) );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsEditor
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsEditor, CPropertyPage)
	ON_BN_CLICKED(IDC_USEAUTOCOMPLETE_CHECK, OnUseAutoCompleteCheck)
	ON_BN_CLICKED(IDC_SHOWFULLLIST_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_CLEARAUTO_CHECK, OnClearAutoCheck)
	ON_BN_CLICKED(IDC_SHOWNEARESTWORDSONLY_RADIO, OnUpdateModify)
	ON_EN_CHANGE(IDC_CLEARAUTO_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARGIN_FOLD_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARGIN_BOOKMARK_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARGIN_LINENUMBER_CHECK, OnUpdateModify)
END_MESSAGE_MAP()

RDOStudioOptionsEditor::RDOStudioOptionsEditor( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	m_bufferClearAuto = FALSE;
	m_bufferDelay = 0;
	m_codecompUse = FALSE;
	m_codecompShowFullList = -1;
	m_marginFold = FALSE;
	m_marginBookmark = FALSE;
	m_marginLineNumber = FALSE;

	m_bufferClearAuto = sheet->style_editor.buffer->canClearBuffer ? 1 : 0;
	m_bufferDelay     = sheet->style_editor.buffer->clearBufferDelay;

	m_codecompUse          = sheet->style_editor.autoComplete->useAutoComplete ? 1 : 0;
	m_codecompShowFullList = sheet->style_editor.autoComplete->showFullList ? 0 : 1;

	m_marginFold       = sheet->style_editor.margin->fold ? 1 : 0;
	m_marginBookmark   = sheet->style_editor.margin->bookmark ? 1 : 0;
	m_marginLineNumber = sheet->style_editor.margin->lineNumber ? 1 : 0;

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioOptionsEditor::~RDOStudioOptionsEditor()
{
}

void RDOStudioOptionsEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CLEARAUTO_CHECK, m_bufferClearAuto);
	DDX_Text(pDX, IDC_CLEARAUTO_EDIT, m_bufferDelay);
	DDV_MinMaxInt(pDX, m_bufferDelay, 1, 100);
	DDX_Check(pDX, IDC_USEAUTOCOMPLETE_CHECK, m_codecompUse);
	DDX_Radio(pDX, IDC_SHOWFULLLIST_RADIO, m_codecompShowFullList);
	DDX_Check(pDX, IDC_MARGIN_FOLD_CHECK, m_marginFold);
	DDX_Check(pDX, IDC_MARGIN_BOOKMARK_CHECK, m_marginBookmark);
	DDX_Check(pDX, IDC_MARGIN_LINENUMBER_CHECK, m_marginLineNumber);
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

BOOL RDOStudioOptionsEditor::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioOptionsEditor::OnClearAutoCheck() 
{
	rbool use = static_cast<CButton*>(GetDlgItem( IDC_CLEARAUTO_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_CLEARAUTO_STATIC1 )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_CLEARAUTO_STATIC2 )->EnableWindow( use );
	OnUpdateModify();
}

void RDOStudioOptionsEditor::OnUseAutoCompleteCheck() 
{
	rbool use = static_cast<CButton*>(GetDlgItem( IDC_USEAUTOCOMPLETE_CHECK ))->GetCheck() ? true : false;
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

	SetModified( *sheet->style_editor.buffer != *studioApp.m_pMainFrame->style_editor.buffer || *sheet->style_editor.autoComplete != *studioApp.m_pMainFrame->style_editor.autoComplete || *sheet->style_editor.margin != *studioApp.m_pMainFrame->style_editor.margin );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsTabs
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsTabs, CPropertyPage)
	ON_BN_CLICKED(IDC_USETABS_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_TABSIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_TABINDENTS_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_INDENTSIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_BACKSPACEUNTABS_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_BACKSPACEUNINDENTS_RADIO, OnUpdateModify)
	ON_BN_CLICKED(IDC_AUTOINDENT_CHECK, OnUpdateModify)
END_MESSAGE_MAP()

RDOStudioOptionsTabs::RDOStudioOptionsTabs( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	m_tabUse = FALSE;
	m_tabSize = 0;
	m_tabUseTabIndent = FALSE;
	m_tabIndentSize = 0;
	m_tabBackspaceUntabs = -1;
	m_tabAutoIndent = FALSE;

	m_tabUse             = sheet->style_editor.tab->useTabs ? 1 : 0;
	m_tabSize            = sheet->style_editor.tab->tabSize;
	m_tabUseTabIndent    = sheet->style_editor.tab->tabIndents ? 1 : 0;
	m_tabIndentSize      = sheet->style_editor.tab->indentSize;
	m_tabBackspaceUntabs = sheet->style_editor.tab->backspaceUntabs ? 0 : 1;
	m_tabAutoIndent      = sheet->style_editor.tab->autoIndent ? 1 : 0;

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioOptionsTabs::~RDOStudioOptionsTabs()
{
}

void RDOStudioOptionsTabs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_USETABS_CHECK, m_tabUse);
	DDX_Text(pDX, IDC_TABSIZE_EDIT, m_tabSize);
	DDV_MinMaxInt(pDX, m_tabSize, 1, 100);
	DDX_Radio(pDX, IDC_BACKSPACEUNTABS_RADIO, m_tabBackspaceUntabs);
	DDX_Text(pDX, IDC_INDENTSIZE_EDIT, m_tabIndentSize);
	DDV_MinMaxInt(pDX, m_tabIndentSize, 1, 100);
	DDX_Check(pDX, IDC_AUTOINDENT_CHECK, m_tabAutoIndent);
	DDX_Check(pDX, IDC_TABINDENTS_CHECK, m_tabUseTabIndent);
}

void RDOStudioOptionsTabs::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioOptionsTabs::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
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

	SetModified( *sheet->style_editor.tab != *studioApp.m_pMainFrame->style_editor.tab );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsColorsStyles
// --------------------------------------------------------------------------------
COLORREF RDOStudioOptionsColorsStyles::null_fg_color = RGB( 0x00, 0x00, 0x00 );
COLORREF RDOStudioOptionsColorsStyles::null_bg_color = RGB( 0xFF, 0xFF, 0xFF );
rbool RDOStudioOptionsColorsStyles::null_wordwrap      = false;
rbool RDOStudioOptionsColorsStyles::null_horzscrollbar = true;
rbool RDOStudioOptionsColorsStyles::null_warning       = true;
rbool RDOStudioOptionsColorsStyles::null_commentfold   = false;
RDOBookmarkStyle RDOStudioOptionsColorsStyles::null_bookmarkstyle = RDOBOOKMARKS_NONE;
RDOFoldStyle     RDOStudioOptionsColorsStyles::null_foldstyle     = RDOFOLDS_NONE;

BEGIN_MESSAGE_MAP(RDOStudioOptionsColorsStyles, CPropertyPage)
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
	ON_CBN_SELCHANGE(IDC_BOOKMARK_COMBO, OnBookmarkChanged)
	ON_CBN_SELCHANGE(IDC_FOLD_COMBO, OnFoldChanged)
	ON_CBN_SELCHANGE(IDC_THEME_COMBO, OnThemeChanged)
	ON_EN_CHANGE(IDC_VERTBORDER_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_COMMENTGROUP_CHECK, OnCommentGroupCheck)
	ON_EN_CHANGE(IDC_HORZBORDER_EDIT, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_TITLE_FONTSIZE_COMBO, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_LEGEND_FONTSIZE_COMBO, OnUpdateModify)
	ON_EN_CHANGE(IDC_TICKWIDTH_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_WARNING_CHECK, OnWarningCheck)
END_MESSAGE_MAP()

RDOStudioOptionsColorsStyles::RDOStudioOptionsColorsStyles( RDOStudioOptions& _sheet ):
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
	null_font_style( RDOStyleFont::NONE )
{
	STYLEObject* object;
	object = new STYLEObject( STYLEObject::all, all_font_name, all_font_size );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_ALLWINDOW ), null_font_style, all_fg_color, all_bg_color ) );
	objects.push_back( object );

	RDOEditorEditTheme* editor_theme = static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme);
	object = new STYLEObject( STYLEObject::source, sheet->style_editor.font->name, sheet->style_editor.font->size, true, sheet->style_editor.window->wordWrap, sheet->style_editor.window->showHorzScrollBar, editor_theme->bookmarkStyle, editor_theme->foldStyle, editor_theme->commentFold );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR ), editor_theme->identifierStyle, editor_theme->identifierColor, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_PLAINTEXT ), editor_theme->defaultStyle, editor_theme->defaultColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_IDENTIFICATOR ), editor_theme->identifierStyle, editor_theme->identifierColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_KEYWORD ), editor_theme->keywordStyle, editor_theme->keywordColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_FUNCTION ), editor_theme->functionsStyle, editor_theme->functionsColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TRACE ), editor_theme->traceStyle, editor_theme->traceColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_COLOR ), editor_theme->colorStyle, editor_theme->colorColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_COMMENT ), editor_theme->commentStyle, editor_theme->commentColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_NUMBER ), editor_theme->numberStyle, editor_theme->numberColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_STRING ), editor_theme->stringStyle, editor_theme->stringColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_OPERATOR ), editor_theme->operatorStyle, editor_theme->operatorColor, null_bg_color, null_fg_color, editor_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_CARET ), null_font_style, editor_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, editor_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, editor_theme->bookmarkFgColor, editor_theme->bookmarkBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_FOLD ), null_font_style, editor_theme->foldFgColor, editor_theme->foldBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_ERROR ), null_font_style, null_fg_color, editor_theme->errorBgColor ) );
	objects.push_back( object );

	RDOBuildEditTheme* build_theme = static_cast<RDOBuildEditTheme*>(sheet->style_build.theme);
	object = new STYLEObject( STYLEObject::build, sheet->style_build.font->name, sheet->style_build.font->size, false, sheet->style_build.window->wordWrap, sheet->style_build.window->showHorzScrollBar, null_bookmarkstyle, null_foldstyle, null_commentfold, build_theme->warning );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD ), build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD_TEXT ), build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD_SELECTEDLINE ), null_font_style, null_fg_color, build_theme->selectLineBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_CARET ), null_font_style, build_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, build_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, null_fg_color, build_theme->bookmarkBgColor ) );
	objects.push_back( object );

	RDOBaseEditTheme* debug_theme = static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme);
	object = new STYLEObject( STYLEObject::debug, sheet->style_debug.font->name, sheet->style_debug.font->size, false, sheet->style_debug.window->wordWrap, sheet->style_debug.window->showHorzScrollBar );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_DEBUG ), debug_theme->defaultStyle, debug_theme->defaultColor, debug_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD_TEXT ), debug_theme->defaultStyle, debug_theme->defaultColor, debug_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_CARET ), null_font_style, debug_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, debug_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, null_fg_color, debug_theme->bookmarkBgColor ) );
	objects.push_back( object );

	RDOTracerLogTheme* trace_theme = static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme);
	object = new STYLEObject( STYLEObject::trace, sheet->style_trace.font->name, sheet->style_trace.font->size );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG ), trace_theme->style, trace_theme->defaultColor.foregroundColor, trace_theme->defaultColor.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_ES ), trace_theme->style, trace_theme->es.foregroundColor, trace_theme->es.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_EB ), trace_theme->style, trace_theme->eb.foregroundColor, trace_theme->eb.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_EF ), trace_theme->style, trace_theme->ef.foregroundColor, trace_theme->ef.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_EI ), trace_theme->style, trace_theme->ei.foregroundColor, trace_theme->ei.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_ER ), trace_theme->style, trace_theme->er.foregroundColor, trace_theme->er.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_RC ), trace_theme->style, trace_theme->rc.foregroundColor, trace_theme->rc.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_RE ), trace_theme->style, trace_theme->re.foregroundColor, trace_theme->re.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_RK ), trace_theme->style, trace_theme->rk.foregroundColor, trace_theme->rk.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_V ),  trace_theme->style, trace_theme->v.foregroundColor,  trace_theme->v.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_STATUS ), trace_theme->style, trace_theme->s.foregroundColor, trace_theme->s.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_DPS ), trace_theme->style, trace_theme->dps.foregroundColor, trace_theme->dps.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SB ),  trace_theme->style, trace_theme->sb.foregroundColor,  trace_theme->sb.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SO ),  trace_theme->style, trace_theme->so.foregroundColor,  trace_theme->so.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_STN ), trace_theme->style, trace_theme->stn.foregroundColor, trace_theme->stn.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_STD ), trace_theme->style, trace_theme->std.foregroundColor, trace_theme->std.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_STR ), trace_theme->style, trace_theme->str.foregroundColor, trace_theme->str.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SRC ), trace_theme->style, trace_theme->src.foregroundColor, trace_theme->src.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SRE ), trace_theme->style, trace_theme->sre.foregroundColor, trace_theme->sre.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SRK ), trace_theme->style, trace_theme->srk.foregroundColor, trace_theme->srk.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SD ),  trace_theme->style, trace_theme->sd.foregroundColor,  trace_theme->sd.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SES ), trace_theme->style, trace_theme->ses.foregroundColor, trace_theme->ses.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SEN ), trace_theme->style, trace_theme->sen.foregroundColor, trace_theme->sen.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SEM ), trace_theme->style, trace_theme->sem.foregroundColor, trace_theme->sem.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SEF ), trace_theme->style, trace_theme->sef.foregroundColor, trace_theme->sef.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_LOG_SEU ), trace_theme->style, trace_theme->seu.foregroundColor, trace_theme->seu.backgroundColor ) );
	objects.push_back( object );

	RDOEditorBaseEditTheme* results_theme = static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme);
	object = new STYLEObject( STYLEObject::results, sheet->style_results.font->name, sheet->style_results.font->size, true, sheet->style_results.window->wordWrap, sheet->style_results.window->showHorzScrollBar );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_RESULTS ), results_theme->identifierStyle, results_theme->identifierColor, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_PLAINTEXT ), results_theme->defaultStyle, results_theme->defaultColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_IDENTIFICATOR ), results_theme->identifierStyle, results_theme->identifierColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_KEYWORD ), results_theme->keywordStyle, results_theme->keywordColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
//	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_FUNCTION ), results_theme->functionsStyle, results_theme->functionsColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
//	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TRACE ), results_theme->traceStyle, results_theme->traceColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
//	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_COLOR ), results_theme->colorStyle, results_theme->colorColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
//	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_COMMENT ), results_theme->commentStyle, results_theme->commentColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_NUMBER ), results_theme->numberStyle, results_theme->numberColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_STRING ), results_theme->stringStyle, results_theme->stringColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_OPERATOR ), results_theme->operatorStyle, results_theme->operatorColor, null_bg_color, null_fg_color, results_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_CARET ), null_font_style, results_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, results_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, results_theme->bookmarkFgColor, results_theme->bookmarkBgColor ) );
	objects.push_back( object );

	RDOFindEditTheme* find_theme = static_cast<RDOFindEditTheme*>(sheet->style_find.theme);
	object = new STYLEObject( STYLEObject::find, sheet->style_find.font->name, sheet->style_find.font->size, true, sheet->style_find.window->wordWrap, sheet->style_find.window->showHorzScrollBar );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_FIND ), find_theme->defaultStyle, find_theme->defaultColor, find_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD_TEXT ), find_theme->defaultStyle, find_theme->defaultColor, find_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_FIND_SEARCHTEXT ), find_theme->keywordStyle, find_theme->keywordColor, null_bg_color, null_fg_color, find_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_BUILD_SELECTEDLINE ), null_font_style, null_fg_color, find_theme->selectLineBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_CARET ), null_font_style, find_theme->caretColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_TEXTSELECTION ), null_font_style, null_fg_color, find_theme->selectionBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_EDITOR_BOOKMARK ), null_font_style, null_fg_color, find_theme->bookmarkBgColor ) );
	objects.push_back( object );

	RDOStudioChartViewTheme* chart_theme = static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme);
	object = new STYLEObject( STYLEObject::chart, sheet->style_chart.font->name, sheet->style_chart.font->size, false );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART ), chart_theme->defaultStyle, chart_theme->defaultColor, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART_AXIS ), chart_theme->defaultStyle, chart_theme->axisFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART_TITLE ), chart_theme->titleStyle, chart_theme->titleFGColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART_LEGEND ), chart_theme->legendStyle, chart_theme->legendFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART_CHART ), null_font_style, null_fg_color, chart_theme->chartBgColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_CHART_TIME ), null_font_style, null_fg_color, chart_theme->timeBgColor ) );
	objects.push_back( object );

	RDOStudioFrameTheme* frame_theme = static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme);
	object = new STYLEObject( STYLEObject::frame, sheet->style_frame.font->name, sheet->style_frame.font->size, false );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_FRAME ), frame_theme->defaultStyle, frame_theme->defaultColor, frame_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_FRAME_BORDER ), frame_theme->defaultStyle, frame_theme->defaultColor, null_bg_color ) );
	object->properties.push_back( new STYLEProperty( object, rdo::format( IDS_COLORSTYLE_FRAME_BACKGROUND ), frame_theme->defaultStyle, null_fg_color, frame_theme->backgroundColor ) );
	objects.push_back( object );

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioOptionsColorsStyles::~RDOStudioOptionsColorsStyles()
{
	std::list< STYLEObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		delete *it++;
	};
}

void RDOStudioOptionsColorsStyles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WARNING_CHECK, m_warning);
	DDX_Control(pDX, IDC_COMMENTGROUP_CHECK, m_commentGroupButton);
	DDX_Control(pDX, IDC_TITLE_FONTSIZE_STATIC, m_title_fontSizeStatic);
	DDX_Control(pDX, IDC_LEGEND_FONTSIZE_STATIC, m_leg_fontSizeStatic);
	DDX_Control(pDX, IDC_LEGEND_FONTSIZE_COMBO, m_leg_fontSizeCombo);
	DDX_Control(pDX, IDC_TITLE_FONTSIZE_COMBO, m_title_fontSizeCombo);
	DDX_Control(pDX, IDC_TICKWIDTH_STATIC, m_tickWidthStatic);
	DDX_Control(pDX, IDC_TICKWIDTH_EDIT, m_tickWidth);
	DDX_Control(pDX, IDC_VERTBORDER_STATIC, m_vertBorderStatic);
	DDX_Control(pDX, IDC_VERTBORDER_EDIT, m_vertBorder);
	DDX_Control(pDX, IDC_HORZBORDER_STATIC, m_horzBorderStatic);
	DDX_Control(pDX, IDC_HORZBORDER_EDIT, m_horzBorder);
	DDX_Control(pDX, IDC_THEME_COMBO, m_theme);
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

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );
}

int CALLBACK RDOStudioOptionsColorsStyles::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam )
{
	std::list< STYLEFont >* fonts = reinterpret_cast< std::list< STYLEFont >* >(lParam);
	rbool can_insert = true;
	std::list< STYLEFont >::iterator font_it = fonts->begin();
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

BOOL RDOStudioOptionsColorsStyles::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = static_cast<CComboBox*>(GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	sheet->preview_editor.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_editor.setEditorStyle( &sheet->style_editor );
	sheet->preview_editor.setCanClearErrorLine( false );
	sheet->preview_editor.appendText( rdo::format( IDS_COLORSTYLE_EDITOR_SAMPLE ) );
	sheet->preview_editor.scrollToLine( 0 );
	sheet->preview_editor.setReadOnly( true );
	sheet->preview_editor.bookmarkToggle();
	sheet->preview_editor.setErrorLine( sheet->preview_editor.getLineCount() - 1 );

	sheet->preview_build.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_build.setEditorStyle( &sheet->style_build );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( rdo::format( IDS_COLORSTYLE_BUILD_SAMPLE1 ) ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( rdoSimulator::RDOSyntaxError::UNKNOWN, rdo::format( IDS_COLORSTYLE_BUILD_SAMPLE2 ), rdoModelObjects::PAT, 40, true ) );
	sheet->preview_build.appendLine( new RDOBuildEditLineInfo( rdo::format( IDS_COLORSTYLE_BUILD_SAMPLE3 ) ) );
	sheet->preview_build.gotoNext();

	sheet->preview_debug.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_debug.setEditorStyle( &sheet->style_debug );
	sheet->preview_debug.appendLine( rdo::format( IDS_COLORSTYLE_DEBUG_SAMPLE ) );

	sheet->preview_trace.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_trace.setStyle( &sheet->style_trace );
	sheet->preview_trace.setFocusOnly( true );
	sheet->preview_trace.setShowMenu( false );
	sheet->preview_trace.setText( rdo::format( IDS_COLORSTYLE_LOG_SAMPLE ) );
	sheet->preview_trace.selectLine( 0 );

	sheet->preview_results.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_results.setEditorStyle( &sheet->style_results );
	sheet->preview_results.setReadOnly( false );
	sheet->preview_results.replaceCurrent( rdo::format( IDS_COLORSTYLE_RESULTS_SAMPLE ), 0 );
	sheet->preview_results.setReadOnly( true );

	sheet->preview_find.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_find.setEditorStyle( &sheet->style_find );
	sheet->preview_find.setKeyword( "$Time" );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( rdo::format( IDS_COLORSTYLE_FIND_SAMPLE1 ) ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( rdo::format( IDS_COLORSTYLE_FIND_SAMPLE2 ), rdoModelObjects::PAT, 3 ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( rdo::format( IDS_COLORSTYLE_FIND_SAMPLE3 ), rdoModelObjects::PAT, 13 ) );
	sheet->preview_find.appendLine( new RDOLogEditLineInfo( rdo::format( IDS_COLORSTYLE_FIND_SAMPLE4 ) ) );
	sheet->preview_find.gotoNext();

	sheet->preview_chart_doc->SetTitle( rdo::format( IDS_COLORSTYLE_CHART_SAMPLE1 ).c_str() );
	sheet->preview_chart->previewMode = true;
	sheet->preview_chart->Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_chart_doc->AddView( sheet->preview_chart );
	sheet->preview_chart->setStyle( &sheet->style_chart, false );
	//initializing times vector
	sheet->preview_times.push_back( RDOTracerTimeNow( 0, 3 ) );
	sheet->preview_times.push_back( RDOTracerTimeNow( 2, 3 ) );
	sheet->preview_times.push_back( RDOTracerTimeNow( 4, 3 ) );
	sheet->preview_times.push_back( RDOTracerTimeNow( 6, 3 ) );
	sheet->preview_times.push_back( RDOTracerTimeNow( 8, 3 ) );
	sheet->preview_times.push_back( RDOTracerTimeNow( 10, 3 ) );
	sheet->preview_serie.setTitle( rdo::format( IDS_COLORSTYLE_CHART_SAMPLE2 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 0 ), 2, 0 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 1 ), 1, 1 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 2 ), 0, 4 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 3 ), 3, 3 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 4 ), 1, 2 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 5 ), 0, 3 ) );
	sheet->preview_chart_doc->addSerie( &sheet->preview_serie );
	sheet->chart_need_delete = false;

	sheet->preview_frame.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_frame.setStyle( &sheet->style_frame );

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
	sheet->preview_chart->MoveWindow( rectEdit );
	sheet->preview_frame.MoveWindow( rectEdit );

	std::list< STYLEObject* >::const_iterator obj_it = objects.begin();
	while ( obj_it != objects.end() ) {
		std::list< STYLEProperty* >::iterator prop_it = (*obj_it)->properties.begin();
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
	lf.lfCharSet = static_cast<rbyte>(sheet->style_editor.font->characterSet);
	lf.lfFaceName[0] = '\0';
	CDC* dc = GetDC();
	::EnumFontFamiliesEx( dc->m_hDC, &lf, reinterpret_cast<FONTENUMPROC>(RDOStudioOptionsColorsStyles::EnumFontFamExProc), reinterpret_cast<LPARAM>(&fonts), 0 );
	ReleaseDC( dc );
	isCurrentFixed = false;
	loadFontsIntoCombo( true );

	int vertBorder = sheet->style_trace.borders->vertBorder;
	int horzBorder = sheet->style_trace.borders->horzBorder;
	int title_font_size = sheet->style_chart.fonts_ticks->titleFontSize;
	int legent_font_size = sheet->style_chart.fonts_ticks->legendFontSize;
	int tick_width = sheet->style_chart.fonts_ticks->tickWidth;
	m_vertBorder.SetWindowText( rdo::format( "%d", vertBorder ).c_str() );
	m_horzBorder.SetWindowText( rdo::format( "%d", horzBorder ).c_str() );
	m_vertBorder.SetLimitText( 2 );
	m_horzBorder.SetLimitText( 2 );

	int index = m_title_fontSizeCombo.FindStringExact( -1, rdo::format( "%d", title_font_size ).c_str() );
	if ( index != CB_ERR ) {
		m_title_fontSizeCombo.SetCurSel( index );
	} else {
		m_title_fontSizeCombo.SetCurSel( -1 );
	}
	index = m_leg_fontSizeCombo.FindStringExact( -1, rdo::format( "%d", legent_font_size ).c_str() );
	if ( index != CB_ERR ) {
		m_leg_fontSizeCombo.SetCurSel( index );
	} else {
		m_leg_fontSizeCombo.SetCurSel( -1 );
	}
	m_tickWidth.SetWindowText( rdo::format( "%d", tick_width ).c_str() );
	m_tickWidth.SetLimitText( 2 );
	
	setPreviewAsCombo( STYLEObject::source );

	updatePropOfAllObject();
	m_styleItem.SelectItem( m_styleItem.GetRootItem() );

	return true;
}

void RDOStudioOptionsColorsStyles::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioOptionsColorsStyles::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioOptionsColorsStyles::OnStyleItemChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	updateStyleItem();

	*pResult = 0;
}

void RDOStudioOptionsColorsStyles::updateStyleItem()
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
		std::list< STYLEFont >::const_iterator it = fonts.begin();
		rbool flag = false;
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
		int index = m_fontSize.FindStringExact( -1, rdo::format( "%d", prop->object->font_size ).c_str() );
		if ( index != CB_ERR ) {
			m_fontSize.SetCurSel( index );
		} else {
			m_fontSize.SetCurSel( -1 );
		}

		// Update font style checkboxs
		rbool flag_font_style = &prop->font_style != &null_font_style;
		m_fontStyleBold.EnableWindow( flag_font_style );
		m_fontStyleItalic.EnableWindow( flag_font_style );
		m_fontStyleUnderline.EnableWindow( flag_font_style );
		m_fontStyleBold.SetCheck( prop->font_style & RDOStyleFont::BOLD );
		m_fontStyleItalic.SetCheck( prop->font_style & RDOStyleFont::ITALIC );
		m_fontStyleUnderline.SetCheck( prop->font_style & RDOStyleFont::UNDERLINE );

		// Update FG color combobox
		rbool flag_fg_color = &prop->fg_color != &null_fg_color;
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
		rbool flag_bg_color = &prop->bg_color != &null_bg_color;
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
		rbool flag_wordwrap = type == STYLEObject::source || type == STYLEObject::build || type == STYLEObject::debug || type == STYLEObject::results || type == STYLEObject::find;
		m_wordWrap.ShowWindow( flag_wordwrap ? SW_SHOW : SW_HIDE );
		m_horzScrollBar.ShowWindow( flag_wordwrap ? SW_SHOW : SW_HIDE );
		m_wordWrap.SetCheck( prop->object->wordwrap ? 1 : 0 );
		m_horzScrollBar.SetCheck( prop->object->horzscrollbar ? 1 : 0 );
		OnWordWrapClicked();

		// Update warning
		rbool flag_worning = type == STYLEObject::build;
		m_warning.ShowWindow( flag_worning ? SW_SHOW : SW_HIDE );
		m_warning.SetCheck( prop->object->warning ? 1 : 0 );
		OnWordWrapClicked();

		// Update borders
		rbool flag_border = type == STYLEObject::trace;
		m_vertBorder.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_vertBorderStatic.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_horzBorder.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_horzBorderStatic.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );

		// Update fonts and ticks
		rbool flag_fonts_ticks = type == STYLEObject::chart;
		m_title_fontSizeStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_leg_fontSizeStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_leg_fontSizeCombo.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_title_fontSizeCombo.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_tickWidth.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_tickWidthStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );

		// Update bookmark
		rbool flag_bookmark = type == STYLEObject::source;
		m_bookmark.ShowWindow( flag_bookmark ? SW_SHOW : SW_HIDE );
		m_bookmarkStatic.ShowWindow( flag_bookmark ? SW_SHOW : SW_HIDE );
		m_bookmark.SetCurSel( prop->object->bookmarkstyle );

		// Update fold
		rbool flag_fold = type == STYLEObject::source;
		m_fold.ShowWindow( flag_fold ? SW_SHOW : SW_HIDE );
		m_foldStatic.ShowWindow( flag_fold ? SW_SHOW : SW_HIDE );
		m_commentGroupButton.ShowWindow( flag_fold ? SW_SHOW : SW_HIDE );
		m_fold.SetCurSel( prop->object->foldstyle );
		m_commentGroupButton.SetCheck( prop->object->commentfold );
		OnFoldChanged();

		// Update preview
		rbool flag_preview = type == STYLEObject::all;
		m_previewAs.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		m_previewAsStatic.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		setPreviewAsCombo( type );

		// Update theme
		m_theme.ResetContent();
		m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_CURRENT ).c_str() );
		m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_DEFAULT ).c_str() );
		switch ( type ) {
			case STYLEObject::all: {
				break;
			}
			case STYLEObject::source:
			case STYLEObject::results: {
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_CPP ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_PASCAL ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_HTML ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_CLASSIC ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_TWILIGHT ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_OCEAN ).c_str() );
				break;
			}
			case STYLEObject::build:
			case STYLEObject::debug:
			case STYLEObject::find: {
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_CLASSIC ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_TWILIGHT ).c_str() );
				m_theme.AddString( rdo::format( IDS_COLORSTYLE_EDITOR_THEME_OCEAN ).c_str() );
				break;
			}
			case STYLEObject::trace: {
				break;
			}
			case STYLEObject::chart: {
				break;
			}
			default: break;
		}
		updateTheme();
	}
}

void RDOStudioOptionsColorsStyles::updateTheme()
{
	switch ( getCurrentObjectType() ) {
		case STYLEObject::all: {
			if ( *static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme) == RDOEditorEditTheme::getDefaultTheme() &&
				 *static_cast<RDOBuildEditTheme*>(sheet->style_build.theme) == RDOBuildEditTheme::getDefaultTheme() &&
				 *static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme) == RDOBaseEditTheme::getDefaultTheme() &&
				 *static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) == RDOTracerLogTheme::getDefaultTheme() &&
				 *static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme) == RDOEditorBaseEditTheme::getDefaultTheme() &&
				 *static_cast<RDOFindEditTheme*>(sheet->style_find.theme) == RDOFindEditTheme::getDefaultTheme() &&
				 *static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) == RDOStudioChartViewTheme::getDefaultTheme() &&
				 *static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme) == RDOStudioFrameTheme::getDefaultTheme() &&
				 *sheet->style_editor.font == RDOStyleFont::getDefaultFont() &&
				 *sheet->style_build.font == RDOStyleFont::getDefaultFont() &&
				 *sheet->style_debug.font == RDOStyleFont::getDefaultFont() &&
				 *sheet->style_trace.font == RDOStyleFont::getTracerLogFont() &&
				 *sheet->style_results.font == RDOStyleFont::getDefaultFont() &&
				 *sheet->style_find.font == RDOStyleFont::getDefaultFont() &&
				 *sheet->style_chart.font == RDOStyleFont::getChartViewFont() &&
				 *sheet->style_frame.font == RDOStyleFont::getFrameFont() ) {
				m_theme.SetCurSel( 1 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::source: {
			RDOEditorEditTheme* theme = static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme);
			RDOStyleFont* font = sheet->style_editor.font;
			if ( *theme == RDOEditorEditTheme::getDefaultTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 1 );
			} else if ( *theme == RDOEditorEditTheme::getCppTheme() && *font == RDOStyleFont::getDefaultFont()  ) {
				m_theme.SetCurSel( 2 );
			} else if ( *theme == RDOEditorEditTheme::getPascalTheme() && *font == RDOStyleFont::getDefaultFont()  ) {
				m_theme.SetCurSel( 3 );
			} else if ( *theme == RDOEditorEditTheme::getHtmlTheme() && *font == RDOStyleFont::getDefaultFont()  ) {
				m_theme.SetCurSel( 4 );
			} else if ( *theme == RDOEditorEditTheme::getClassicTheme() && *font == RDOStyleFont::getClassicFont()  ) {
				m_theme.SetCurSel( 5 );
			} else if ( *theme == RDOEditorEditTheme::getTwilightTheme() && *font == RDOStyleFont::getDefaultFont()  ) {
				m_theme.SetCurSel( 6 );
			} else if ( *theme == RDOEditorEditTheme::getOceanTheme() && *font == RDOStyleFont::getDefaultFont()  ) {
				m_theme.SetCurSel( 7 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::build: {
			RDOBuildEditTheme* theme = static_cast<RDOBuildEditTheme*>(sheet->style_build.theme);
			RDOStyleFont* font = sheet->style_build.font;
			if ( *theme == RDOBuildEditTheme::getDefaultTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 1 );
			} else if ( *theme == RDOBuildEditTheme::getClassicTheme() && *font == RDOStyleFont::getClassicFont() ) {
				m_theme.SetCurSel( 2 );
			} else if ( *theme == RDOBuildEditTheme::getTwilightTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 3 );
			} else if ( *theme == RDOBuildEditTheme::getOceanTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 4 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::debug: {
			RDOBaseEditTheme* theme = static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme);
			RDOStyleFont* font = sheet->style_debug.font;
			if ( *theme == RDOBaseEditTheme::getDefaultTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 1 );
			} else if ( *theme == RDOBaseEditTheme::getClassicTheme() && *font == RDOStyleFont::getClassicFont() ) {
				m_theme.SetCurSel( 2 );
			} else if ( *theme == RDOBaseEditTheme::getTwilightTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 3 );
			} else if ( *theme == RDOBaseEditTheme::getOceanTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 4 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::trace: {
			RDOTracerLogTheme* theme = static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme);
			RDOStyleFont* font = sheet->style_trace.font;
			if ( *theme == RDOTracerLogTheme::getDefaultTheme() && *font == RDOStyleFont::getTracerLogFont() ) {
				m_theme.SetCurSel( 1 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::results: {
			RDOEditorBaseEditTheme* theme = static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme);
			RDOStyleFont* font = sheet->style_results.font;
			if ( *theme == RDOEditorBaseEditTheme::getDefaultTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 1 );
			} else if ( *theme == RDOEditorBaseEditTheme::getCppTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 2 );
			} else if ( *theme == RDOEditorBaseEditTheme::getPascalTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 3 );
			} else if ( *theme == RDOEditorBaseEditTheme::getHtmlTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 4 );
			} else if ( *theme == RDOEditorBaseEditTheme::getClassicTheme() && *font == RDOStyleFont::getClassicFont() ) {
				m_theme.SetCurSel( 5 );
			} else if ( *theme == RDOEditorBaseEditTheme::getTwilightTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 6 );
			} else if ( *theme == RDOEditorBaseEditTheme::getOceanTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 7 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::find: {
			RDOFindEditTheme* theme = static_cast<RDOFindEditTheme*>(sheet->style_find.theme);
			RDOStyleFont* font = sheet->style_find.font;
			if ( *theme == RDOFindEditTheme::getDefaultTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 1 );
			} else if ( *theme == RDOFindEditTheme::getClassicTheme() && *font == RDOStyleFont::getClassicFont() ) {
				m_theme.SetCurSel( 2 );
			} else if ( *theme == RDOFindEditTheme::getTwilightTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 3 );
			} else if ( *theme == RDOFindEditTheme::getOceanTheme() && *font == RDOStyleFont::getDefaultFont() ) {
				m_theme.SetCurSel( 4 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::chart: {
			RDOStudioChartViewTheme* theme = static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme);
			RDOStyleFont* font = sheet->style_chart.font;
			if ( *theme == RDOStudioChartViewTheme::getDefaultTheme() && *font == RDOStyleFont::getChartViewFont() ) {
				m_theme.SetCurSel( 1 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		case STYLEObject::frame: {
			RDOStudioFrameTheme* theme = static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme);
			RDOStyleFont* font = sheet->style_frame.font;
			if ( *theme == RDOStudioFrameTheme::getDefaultTheme() && *font == RDOStyleFont::getFrameFont() ) {
				m_theme.SetCurSel( 1 );
			} else {
				m_theme.SetCurSel( 0 );
			}
			break;
		}
		default: break;
	}
}

void RDOStudioOptionsColorsStyles::OnThemeChanged()
{
	int index = m_theme.GetCurSel();
	if ( index != CB_ERR ) {
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				if ( index == 1 ) {
					*static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme) = RDOEditorEditTheme::getDefaultTheme();
					*static_cast<RDOBuildEditTheme*>(sheet->style_build.theme) = RDOBuildEditTheme::getDefaultTheme();
					*static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme) = RDOBaseEditTheme::getDefaultTheme();
					*static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) = RDOTracerLogTheme::getDefaultTheme();
					*static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme) = RDOEditorBaseEditTheme::getDefaultTheme();
					*static_cast<RDOFindEditTheme*>(sheet->style_find.theme) = RDOFindEditTheme::getDefaultTheme();
					*static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) = RDOStudioChartViewTheme::getDefaultTheme();
					*static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme) = RDOStudioFrameTheme::getDefaultTheme();
					*sheet->style_editor.font  = RDOStyleFont::getDefaultFont();
					*sheet->style_build.font   = RDOStyleFont::getDefaultFont();
					*sheet->style_debug.font   = RDOStyleFont::getDefaultFont();
					*sheet->style_trace.font   = RDOStyleFont::getTracerLogFont();
					*sheet->style_results.font = RDOStyleFont::getDefaultFont();
					*sheet->style_find.font    = RDOStyleFont::getDefaultFont();
					*sheet->style_chart.font   = RDOStyleFont::getChartViewFont();
					*sheet->style_frame.font   = RDOStyleFont::getFrameFont();
				}
				break;
			}
			case STYLEObject::source: {
				RDOEditorEditTheme* theme = static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme);
				switch ( index ) {
					case 1: *theme = RDOEditorEditTheme::getDefaultTheme();  *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
					case 2: *theme = RDOEditorEditTheme::getCppTheme();      *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
					case 3: *theme = RDOEditorEditTheme::getPascalTheme();   *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
					case 4: *theme = RDOEditorEditTheme::getHtmlTheme();     *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
					case 5: *theme = RDOEditorEditTheme::getClassicTheme();  *sheet->style_editor.font = RDOStyleFont::getClassicFont(); break;
					case 6: *theme = RDOEditorEditTheme::getTwilightTheme(); *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
					case 7: *theme = RDOEditorEditTheme::getOceanTheme();    *sheet->style_editor.font = RDOStyleFont::getDefaultFont(); break;
				}
				break;
			}
			case STYLEObject::build: {
				RDOBuildEditTheme* theme = static_cast<RDOBuildEditTheme*>(sheet->style_build.theme);
				switch ( index ) {
					case 1: *theme = RDOBuildEditTheme::getDefaultTheme();  *sheet->style_build.font = RDOStyleFont::getDefaultFont(); break;
					case 2: *theme = RDOBuildEditTheme::getClassicTheme();  *sheet->style_build.font = RDOStyleFont::getClassicFont(); break;
					case 3: *theme = RDOBuildEditTheme::getTwilightTheme(); *sheet->style_build.font = RDOStyleFont::getDefaultFont(); break;
					case 4: *theme = RDOBuildEditTheme::getOceanTheme();    *sheet->style_build.font = RDOStyleFont::getDefaultFont(); break;
				}
				break;
			}
			case STYLEObject::debug: {
				RDOBaseEditTheme* theme = static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme);
				switch ( index ) {
					case 1: *theme = RDOBaseEditTheme::getDefaultTheme();  *sheet->style_debug.font = RDOStyleFont::getDefaultFont(); break;
					case 2: *theme = RDOBaseEditTheme::getClassicTheme();  *sheet->style_debug.font = RDOStyleFont::getClassicFont(); break;
					case 3: *theme = RDOBaseEditTheme::getTwilightTheme(); *sheet->style_debug.font = RDOStyleFont::getDefaultFont(); break;
					case 4: *theme = RDOBaseEditTheme::getOceanTheme();    *sheet->style_debug.font = RDOStyleFont::getDefaultFont(); break;
				}
				break;
			}
			case STYLEObject::trace: {
				RDOTracerLogTheme* theme = static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme);
				switch ( index ) {
					case 1: *theme = RDOTracerLogTheme::getDefaultTheme(); *sheet->style_trace.font = RDOStyleFont::getTracerLogFont(); break;
				}
				break;
			}
			case STYLEObject::results: {
				RDOEditorBaseEditTheme* theme = static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme);
				switch ( index ) {
					case 1: *theme = RDOEditorBaseEditTheme::getDefaultTheme();  *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
					case 2: *theme = RDOEditorBaseEditTheme::getCppTheme();      *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
					case 3: *theme = RDOEditorBaseEditTheme::getPascalTheme();   *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
					case 4: *theme = RDOEditorBaseEditTheme::getHtmlTheme();     *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
					case 5: *theme = RDOEditorBaseEditTheme::getClassicTheme();  *sheet->style_results.font = RDOStyleFont::getClassicFont(); break;
					case 6: *theme = RDOEditorBaseEditTheme::getTwilightTheme(); *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
					case 7: *theme = RDOEditorBaseEditTheme::getOceanTheme();    *sheet->style_results.font = RDOStyleFont::getDefaultFont(); break;
				}
				break;
			}
			case STYLEObject::find: {
				RDOFindEditTheme* theme = static_cast<RDOFindEditTheme*>(sheet->style_find.theme);
				switch ( index ) {
					case 1: *theme = RDOFindEditTheme::getDefaultTheme();  *sheet->style_find.font = RDOStyleFont::getDefaultFont(); break;
					case 2: *theme = RDOFindEditTheme::getClassicTheme();  *sheet->style_find.font = RDOStyleFont::getClassicFont(); break;
					case 3: *theme = RDOFindEditTheme::getTwilightTheme(); *sheet->style_find.font = RDOStyleFont::getDefaultFont(); break;
					case 4: *theme = RDOFindEditTheme::getOceanTheme();    *sheet->style_find.font = RDOStyleFont::getDefaultFont(); break;
				}
				break;
			}
			case STYLEObject::chart: {
				RDOStudioChartViewTheme* theme = static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme);
				switch ( index ) {
					case 1: *theme = RDOStudioChartViewTheme::getDefaultTheme(); *sheet->style_chart.font = RDOStyleFont::getChartViewFont(); break;
				}
				break;
			}
			case STYLEObject::frame: {
				RDOStudioFrameTheme* theme = static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme);
				switch ( index ) {
					case 1: *theme = RDOStudioFrameTheme::getDefaultTheme(); *sheet->style_frame.font = RDOStyleFont::getFrameFont(); break;
				}
				break;
			}
			default: break;
		}
		updatePropOfAllObject();
		updateStyleItem();
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFontNameChanged()
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
				sheet->style_chart.font->name   = all_font_name;
				sheet->style_frame.font->name   = all_font_name;
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
			case STYLEObject::chart: {
				sheet->style_chart.font->name = str;
				break;
			}
			case STYLEObject::frame: {
				sheet->style_frame.font->name = str;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFontSizeChanged()
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
				sheet->style_chart.font->size   = all_font_size;
				sheet->style_frame.font->size   = all_font_size;
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
			case STYLEObject::chart: {
				sheet->style_chart.font->size = size;
				break;
			}
			case STYLEObject::frame: {
				sheet->style_frame.font->size = size;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFontStyleBoldChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style & ~RDOStyleFont::BOLD);
		if ( m_fontStyleBold.GetCheck() ) {
			prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style | RDOStyleFont::BOLD);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFontStyleItalicChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style & ~RDOStyleFont::ITALIC);
		if ( m_fontStyleItalic.GetCheck() ) {
			prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style | RDOStyleFont::ITALIC);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFontStyleUnderlineChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( prop && &prop->font_style != &null_font_style ) {
		prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style & ~RDOStyleFont::UNDERLINE);
		if ( m_fontStyleUnderline.GetCheck() ) {
			prop->font_style = static_cast<RDOStyleFont::style>(prop->font_style | RDOStyleFont::UNDERLINE);
		}
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnFgColorChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( &prop->fg_color != &null_fg_color ) {
		prop->fg_color = fgColorCB.getCurrentColor();
		if ( &prop->fg_color == &all_fg_color ) {
			std::list< STYLEObject* >::const_iterator it = objects.begin();
			if ( it != objects.end() ) {
				it++;
				while ( it != objects.end() ) {
					std::list< STYLEProperty* >::const_iterator it_prop = (*it)->properties.begin();
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

void RDOStudioOptionsColorsStyles::OnBgColorChanged()
{
	STYLEProperty* prop = getCurrentProperty();
	if ( &prop->bg_color != &null_bg_color ) {
		prop->bg_color = bgColorCB.getCurrentColor();
		if ( &prop->bg_color == &all_bg_color ) {
			std::list< STYLEObject* >::const_iterator it = objects.begin();
			if ( it != objects.end() ) {
				it++;
				while ( it != objects.end() ) {
					std::list< STYLEProperty* >::const_iterator it_prop = (*it)->properties.begin();
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

void RDOStudioOptionsColorsStyles::OnFgColorClicked()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorChanged();
	}
}

void RDOStudioOptionsColorsStyles::OnBgColorClicked()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorChanged();
	}
}

void RDOStudioOptionsColorsStyles::OnWordWrapClicked()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->wordwrap != &null_wordwrap ) {
		obj->wordwrap = m_wordWrap.GetCheck() ? true : false;
		m_horzScrollBar.EnableWindow( !obj->wordwrap );
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnHorzScrollBarClicked()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->horzscrollbar != &null_horzscrollbar ) {
		obj->horzscrollbar = m_horzScrollBar.GetCheck() ? true : false;
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnWarningCheck()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->warning != &null_warning ) {
		obj->warning = m_warning.GetCheck() ? true : false;
		OnUpdateModify();
	}
}

void RDOStudioOptionsColorsStyles::OnBookmarkChanged()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->bookmarkstyle != &null_bookmarkstyle ) {
		int index = m_bookmark.GetCurSel();
		if ( index != CB_ERR ) {
			obj->bookmarkstyle = static_cast<RDOBookmarkStyle>(index);
			OnUpdateModify();
		}
	}
}

void RDOStudioOptionsColorsStyles::OnFoldChanged()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->foldstyle != &null_foldstyle ) {
		int index = m_fold.GetCurSel();
		if ( index != CB_ERR ) {
			obj->foldstyle = static_cast<RDOFoldStyle>(index);
			m_commentGroupButton.EnableWindow( index != 0 );
			OnUpdateModify();
		}
	}
}

void RDOStudioOptionsColorsStyles::OnPreviewAsChanged()
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
		} else if ( index == 6 ) {
			setPreviewAsCombo( STYLEObject::chart );
		} else if ( index == 7 ) {
			setPreviewAsCombo( STYLEObject::frame );
		}
	}
}

void RDOStudioOptionsColorsStyles::OnUpdateModify()
{
	UpdateData();

	updateTheme();
	updatePropOfAllObject();

	CString str;
	m_vertBorder.GetWindowText( str );
	sheet->style_trace.borders->vertBorder = atoi( str );
	m_horzBorder.GetWindowText( str );
	sheet->style_trace.borders->horzBorder = atoi( str );
	m_tickWidth.GetWindowText( str );
	sheet->style_chart.fonts_ticks->tickWidth = atoi( str );
	if ( sheet->style_chart.fonts_ticks->tickWidth < 2 ) {
		sheet->style_chart.fonts_ticks->tickWidth = 2;
		m_tickWidth.SetWindowText( rdo::format( "%d", sheet->style_chart.fonts_ticks->tickWidth ).c_str() );
	}

	int index = m_leg_fontSizeCombo.GetCurSel();
	if ( index != CB_ERR ) {
		m_leg_fontSizeCombo.GetLBText( index, str );
		sheet->style_chart.fonts_ticks->legendFontSize = atoi( str );
	}
	index = m_title_fontSizeCombo.GetCurSel();
	if ( index != CB_ERR ) {
		m_title_fontSizeCombo.GetLBText( index, str );
		sheet->style_chart.fonts_ticks->titleFontSize = atoi( str );
	}

	sheet->updateStyles();

	SetModified( *sheet->style_editor.font  != *studioApp.m_pMainFrame->style_editor.font ||
	             *sheet->style_build.font   != *studioApp.m_pMainFrame->style_build.font ||
	             *sheet->style_debug.font   != *studioApp.m_pMainFrame->style_debug.font ||
	             *sheet->style_trace.font   != *studioApp.m_pMainFrame->style_trace.font ||
	             *sheet->style_results.font != *studioApp.m_pMainFrame->style_results.font ||
	             *sheet->style_find.font    != *studioApp.m_pMainFrame->style_find.font ||
				 *sheet->style_chart.font   != *studioApp.m_pMainFrame->style_chart.font ||
				 *sheet->style_frame.font   != *studioApp.m_pMainFrame->style_frame.font ||
	             *static_cast<RDOEditorEditTheme*>(sheet->style_editor.theme) != *static_cast<RDOEditorEditTheme*>(studioApp.m_pMainFrame->style_editor.theme) ||
	             *static_cast<RDOBuildEditTheme*>(sheet->style_build.theme) != *static_cast<RDOBuildEditTheme*>(studioApp.m_pMainFrame->style_build.theme) ||
	             *static_cast<RDOBaseEditTheme*>(sheet->style_debug.theme) != *static_cast<RDOBaseEditTheme*>(studioApp.m_pMainFrame->style_debug.theme) ||
	             *static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) != *static_cast<RDOTracerLogTheme*>(studioApp.m_pMainFrame->style_trace.theme) ||
	             *static_cast<RDOEditorBaseEditTheme*>(sheet->style_results.theme) != *static_cast<RDOEditorBaseEditTheme*>(studioApp.m_pMainFrame->style_results.theme) ||
	             *static_cast<RDOFindEditTheme*>(sheet->style_find.theme) != *static_cast<RDOFindEditTheme*>(studioApp.m_pMainFrame->style_find.theme) ||
				 *static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) != *static_cast<RDOStudioChartViewTheme*>(studioApp.m_pMainFrame->style_chart.theme) ||
				 *static_cast<RDOStudioFrameTheme*>(sheet->style_frame.theme) != *static_cast<RDOStudioFrameTheme*>(studioApp.m_pMainFrame->style_frame.theme) ||
	             *sheet->style_editor.window     != *studioApp.m_pMainFrame->style_editor.window ||
	             *sheet->style_build.window      != *studioApp.m_pMainFrame->style_build.window ||
	             *sheet->style_debug.window      != *studioApp.m_pMainFrame->style_debug.window ||
	             *sheet->style_results.window    != *studioApp.m_pMainFrame->style_results.window ||
	             *sheet->style_find.window       != *studioApp.m_pMainFrame->style_find.window ||
	             *sheet->style_trace.borders     != *studioApp.m_pMainFrame->style_trace.borders || 
				 *sheet->style_chart.fonts_ticks != *studioApp.m_pMainFrame->style_chart.fonts_ticks );
}

void RDOStudioOptionsColorsStyles::loadFontsIntoCombo( rbool fixed )
{
	if ( isCurrentFixed != fixed ) {
		m_fontName.ResetContent();
		std::list< STYLEFont >::iterator font_it = fonts.begin();
		while ( font_it != fonts.end() ) {
			if ( !fixed || ( fixed && font_it->fixed ) ) {
				m_fontName.AddString( font_it->name.c_str() );
			}
			font_it++;
		}
		isCurrentFixed = fixed;
	}
}

RDOStudioOptionsColorsStyles::STYLEProperty* RDOStudioOptionsColorsStyles::getCurrentProperty() const
{
	HTREEITEM item = m_styleItem.GetSelectedItem();
	return item ? reinterpret_cast<RDOStudioOptionsColorsStyles::STYLEProperty*>(m_styleItem.GetItemData( item )) : NULL;
}

RDOStudioOptionsColorsStyles::STYLEObject* RDOStudioOptionsColorsStyles::getCurrentObject() const
{
	STYLEProperty* prop = getCurrentProperty();
	return prop ? prop->object : NULL;
}

RDOStudioOptionsColorsStyles::STYLEObject::Type RDOStudioOptionsColorsStyles::getCurrentObjectType() const
{
	STYLEObject* obj = getCurrentObject();
	return obj ? obj->type : STYLEObject::none;
}

void RDOStudioOptionsColorsStyles::setPreviewAsCombo( STYLEObject::Type type )
{
	if ( previewAs != type && type >= STYLEObject::source ) {
		previewAs = type;
		sheet->preview_editor.ShowWindow( SW_HIDE );
		sheet->preview_build.ShowWindow( SW_HIDE );
		sheet->preview_debug.ShowWindow( SW_HIDE );
		sheet->preview_trace.ShowWindow( SW_HIDE );
		sheet->preview_results.ShowWindow( SW_HIDE );
		sheet->preview_find.ShowWindow( SW_HIDE );
		sheet->preview_chart->ShowWindow( SW_HIDE );
		sheet->preview_frame.ShowWindow( SW_HIDE );
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
			case STYLEObject::chart: {
				m_previewAs.SetCurSel( 6 );
				sheet->preview_chart->ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::frame: {
				m_previewAs.SetCurSel( 7 );
				sheet->preview_frame.ShowWindow( SW_SHOW );
				break;
			}
			default: break;
		}
	}
}

void RDOStudioOptionsColorsStyles::updatePropOfAllObject()
{
	std::list< STYLEObject* >::const_iterator it = objects.begin();
	if ( it != objects.end() ) {
		it++;
		if ( it != objects.end() ) {
			all_font_name   = (*it)->font_name;
			all_font_size   = (*it)->font_size;
			rbool use_color = false;
			std::list< STYLEProperty* >::const_iterator prop = (*it)->properties.begin();
			if ( prop != (*it)->properties.end() ) {
				all_fg_color = (*prop)->fg_color;
				all_bg_color = (*prop)->bg_color;
				use_color    = true;
			}
			it++;
			rbool flag_font_name = true;
			rbool flag_font_size = true;
			rbool flag_fg_clor   = true;
			rbool flag_bg_clor   = true;
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

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsPlugins
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptionsPlugins, CPropertyPage)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PLUGIN_LIST, OnPluginListColumnClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLUGIN_LIST, OnPluginListSelectChanged)
	ON_CBN_SELCHANGE(IDC_PLUGIN_RUNMODE_COMBOBOX, OnPluginRunModeComboBoxChanged)
	ON_BN_CLICKED(IDC_PLUGIN_RUNMODE_BUTTON, OnPluginRunModeButtonClicked)
	ON_BN_CLICKED(IDC_PLUGIN_START, OnPluginStart)
	ON_BN_CLICKED(IDC_PLUGIN_STOP, OnPluginStop)
	ON_BN_CLICKED(IDC_PLUGIN_RESTORESTATE_CHECKBOX, OnPluginRestoreStateCheckBoxClicked)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

RDOStudioOptionsPlugins::RDOStudioOptionsPlugins( RDOStudioOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	sortPluginNameAsceding( true ),
	sortPluginVersionAsceding( true ),
	sortPluginRunModeAsceding( true ),
	sortPluginStateAsceding( true ),
	sortPluginDescriptionAsceding( true ),
	timer( 0 )
{
	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioOptionsPlugins::~RDOStudioOptionsPlugins()
{
}

void RDOStudioOptionsPlugins::DoDataExchange(CDataExchange* pDX) 
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGIN_RESTORESTATE_CHECKBOX, m_restoreStateCheckBox);
	DDX_Control(pDX, IDC_PLUGIN_STOP, m_stopButton);
	DDX_Control(pDX, IDC_PLUGIN_START, m_startButton);
	DDX_Control(pDX, IDC_PLUGIN_RUNMODE_COMBOBOX, m_runModeComboBox);
	DDX_Control(pDX, IDC_PLUGIN_RUNMODE_BUTTON, m_runModeButton);
	DDX_Control(pDX, IDC_PLUGIN_RUNMODE_STATIC, m_runModeStatic);
	DDX_Control(pDX, IDC_PLUGIN_LIST, m_pluginList);
}

static int CALLBACK ComparePluginName( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int res = 0;
	RDOStudioPlugin* plugin1 = reinterpret_cast<RDOStudioPlugin*>(lParam1);
	RDOStudioPlugin* plugin2 = reinterpret_cast<RDOStudioPlugin*>(lParam2);
	if ( plugin1 && plugin2 ) {
		res = RDOStudioPlugins::comparePluginsByName( plugin1, plugin2 );
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByVersion( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByRunMode( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByState( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByDescription( plugin1, plugin2 );
		}
		if ( !lParamSort ) {
			res *= -1;
		}
	}
	return res;
}

static int CALLBACK ComparePluginVersion( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int res = 0;
	RDOStudioPlugin* plugin1 = reinterpret_cast<RDOStudioPlugin*>(lParam1);
	RDOStudioPlugin* plugin2 = reinterpret_cast<RDOStudioPlugin*>(lParam2);
	if ( plugin1 && plugin2 ) {
		res = RDOStudioPlugins::comparePluginsByVersion( plugin1, plugin2 );
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByRunMode( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByState( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByDescription( plugin1, plugin2 );
		}
		if ( !lParamSort ) {
			res *= -1;
		}
	}
	return res;
}

static int CALLBACK ComparePluginRunMode( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int res = 0;
	RDOStudioPlugin* plugin1 = reinterpret_cast<RDOStudioPlugin*>(lParam1);
	RDOStudioPlugin* plugin2 = reinterpret_cast<RDOStudioPlugin*>(lParam2);
	if ( plugin1 && plugin2 ) {
		res = RDOStudioPlugins::comparePluginsByRunMode( plugin1, plugin2 );
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByState( plugin1, plugin2 );
		}
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByDescription( plugin1, plugin2 );
		}
		if ( !lParamSort ) {
			res *= -1;
		}
	}
	return res;
}

static int CALLBACK ComparePluginState( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int res = 0;
	RDOStudioPlugin* plugin1 = reinterpret_cast<RDOStudioPlugin*>(lParam1);
	RDOStudioPlugin* plugin2 = reinterpret_cast<RDOStudioPlugin*>(lParam2);
	if ( plugin1 && plugin2 ) {
		res = RDOStudioPlugins::comparePluginsByState( plugin1, plugin2 );
		if ( !res ) {
			res = RDOStudioPlugins::comparePluginsByDescription( plugin1, plugin2 );
		}
		if ( !lParamSort ) {
			res *= -1;
		}
	}
	return res;
}

static int CALLBACK ComparePluginDescription( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int res = 0;
	RDOStudioPlugin* plugin1 = reinterpret_cast<RDOStudioPlugin*>(lParam1);
	RDOStudioPlugin* plugin2 = reinterpret_cast<RDOStudioPlugin*>(lParam2);
	if ( plugin1 && plugin2 ) {
		res = RDOStudioPlugins::comparePluginsByDescription( plugin1, plugin2 );
		if ( !lParamSort ) {
			res *= -1;
		}
	}
	return res;
}

BOOL RDOStudioOptionsPlugins::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_pluginList.InsertColumn( 0, rdo::format( IDS_PLUGIN_NAME ).c_str(), LVCFMT_LEFT, 100 );
	m_pluginList.InsertColumn( 1, rdo::format( IDS_PLUGIN_VERSION ).c_str(), LVCFMT_LEFT, 90, 1 );
	m_pluginList.InsertColumn( 2, rdo::format( IDS_PLUGIN_RUNMODE ).c_str(), LVCFMT_LEFT, 60, 2 );
	m_pluginList.InsertColumn( 3, rdo::format( IDS_PLUGIN_STATE ).c_str(), LVCFMT_LEFT, 50, 3 );
	m_pluginList.InsertColumn( 4, rdo::format( IDS_PLUGIN_DESCRIPTION ).c_str(), LVCFMT_LEFT, 115, 4 );
	m_pluginList.SetExtendedStyle( m_pluginList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	std::vector< RDOStudioPlugin* >::const_iterator it = plugins->getList().begin();
	int i = 0;
	while ( it != plugins->getList().end() ) {
		RDOStudioPlugin* plugin = *it;
		int index = m_pluginList.InsertItem( i, plugin->getName().c_str() );
		if ( index != -1 ) {
			tstring version = rdo::format( "%d.%d (build %d)", plugin->getVersionMajor(), plugin->getVersionMinor(), plugin->getVersionBuild() );
			if ( !plugin->getVersionInfo().empty() ) {
				version += " " + plugin->getVersionInfo();
			}
			m_pluginList.SetItemText( index, 1, version.c_str() );
			updateRunModeInGrid( plugin, index );
			updateStateInGrid( plugin, index );
			m_pluginList.SetItemText( index, 4, plugin->getDescription().c_str() );
			m_pluginList.SetItemData( index, reinterpret_cast<DWORD>(plugin) );
		}
		i++;
		it++;
	}
	if ( i ) {
		m_pluginList.SortItems( ComparePluginName, !NULL );
		m_pluginList.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
	} else {
		updateControls( NULL );
	}

	CRect comboBoxRect;
	m_runModeComboBox.GetWindowRect( comboBoxRect );
	CRect buttonBoxRect;
	m_runModeButton.GetWindowRect( buttonBoxRect );
	ScreenToClient( buttonBoxRect );
	m_runModeButton.MoveWindow( buttonBoxRect.left, buttonBoxRect.top, buttonBoxRect.Width(), comboBoxRect.Height() );

	timer = SetTimer( timerPlugin_ID, 500, NULL );

	return TRUE;
}

void RDOStudioOptionsPlugins::OnDestroy()
{
	if ( timer ) {
		KillTimer( timer );
		timer = 0;
	}
	CPropertyPage::OnDestroy();
}

void RDOStudioOptionsPlugins::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioOptionsPlugins::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioOptionsPlugins::OnPluginListSelectChanged( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	rbool useItem = true;
	if ( !(pNMListView->uChanged & LVIF_STATE) ) {
		useItem = false;
	}

	if ( useItem && (((pNMListView->uNewState & LVIS_SELECTED) != LVIS_SELECTED) || ((pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED)) ) {
		useItem = false;
	}

	rbool havePlugin = false;
	if ( useItem ) {
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(pNMListView->lParam);
		if ( plugin ) {
			updateControls( plugin );
			havePlugin = true;
		}
	}
	if ( !havePlugin ) {
		updateControls( NULL );
	}

	*pResult = 0;
}

void RDOStudioOptionsPlugins::OnPluginListColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	if ( pNMListView->iSubItem == 0 ) {
		sortPluginNameAsceding = !sortPluginNameAsceding;
		m_pluginList.SortItems( ComparePluginName, sortPluginNameAsceding ? !NULL : NULL );
	} else if ( pNMListView->iSubItem == 1 ) {
		sortPluginVersionAsceding = !sortPluginVersionAsceding;
		m_pluginList.SortItems( ComparePluginVersion, sortPluginVersionAsceding ? !NULL : NULL );
	} else if ( pNMListView->iSubItem == 2 ) {
		sortPluginRunModeAsceding = !sortPluginRunModeAsceding;
		m_pluginList.SortItems( ComparePluginRunMode, sortPluginRunModeAsceding ? !NULL : NULL );
	} else if ( pNMListView->iSubItem == 3 ) {
		sortPluginStateAsceding = !sortPluginStateAsceding;
		m_pluginList.SortItems( ComparePluginState, sortPluginStateAsceding ? !NULL : NULL );
	} else if ( pNMListView->iSubItem == 4 ) {
		sortPluginDescriptionAsceding = !sortPluginDescriptionAsceding;
		m_pluginList.SortItems( ComparePluginDescription, sortPluginDescriptionAsceding ? !NULL : NULL );
	}
	
	*pResult = 0;
}

void RDOStudioOptionsPlugins::updateRunModeInGrid( const RDOStudioPlugin* plugin, const int index )
{
	tstring runMode;
	if ( plugin->getRunMode() == rdoPlugin::prmStudioStartUp ) {
		runMode = rdo::format( IDS_PLUGIN_RUNMODE_STUDIOSTARTUP );
	} else if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
		runMode = rdo::format( IDS_PLUGIN_RUNMODE_MODELSTARTUP );
	} else {
		runMode = rdo::format( IDS_PLUGIN_RUNMODE_NOAUTO );
	}
	m_pluginList.SetItemText( index, 2, runMode.c_str() );
}

void RDOStudioOptionsPlugins::updateStateInGrid( const RDOStudioPlugin* plugin, const int index )
{
	m_pluginList.SetItemText( index, 3, rdo::format( plugin->getState() == rdoPlugin::psStoped ? IDS_PLUGIN_STATE_STOPED : IDS_PLUGIN_STATE_ACTIVE ).c_str() );
}

void RDOStudioOptionsPlugins::updateControls( const RDOStudioPlugin* plugin )
{
	if ( plugin ) {
		m_runModeStatic.EnableWindow( true );
		m_runModeComboBox.EnableWindow( true );
		m_runModeButton.EnableWindow( plugin->getRunMode() != plugin->getDefaultRunMode() );
		m_runModeComboBox.SetCurSel( plugin->getRunMode() );
		rbool startPlugin = plugin->getState() == rdoPlugin::psStoped && (plugin->getRunMode() == rdoPlugin::prmNoAuto || plugin->getRunMode() == rdoPlugin::prmStudioStartUp || (plugin->getRunMode() == rdoPlugin::prmModelStartUp && model->isRunning()) );
		rbool stopPlugin  = plugin->getState() == rdoPlugin::psActive && (plugin->getRunMode() == rdoPlugin::prmNoAuto || plugin->getRunMode() == rdoPlugin::prmStudioStartUp || (plugin->getRunMode() == rdoPlugin::prmModelStartUp && model->isRunning()) );
		m_startButton.EnableWindow( startPlugin );
		m_stopButton.EnableWindow( stopPlugin );
		m_restoreStateCheckBox.EnableWindow( plugin->getRunMode() == rdoPlugin::prmNoAuto );
		m_restoreStateCheckBox.SetCheck( plugin->getRestoreState() );
	} else {
		m_runModeStatic.EnableWindow( false );
		m_runModeComboBox.EnableWindow( false );
		m_runModeButton.EnableWindow( false );
		m_startButton.EnableWindow( false );
		m_stopButton.EnableWindow( false );
		m_restoreStateCheckBox.EnableWindow( false );
	}
}

void RDOStudioOptionsPlugins::OnPluginRunModeComboBoxChanged() 
{
	POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
	if ( pos ) {
		int index = m_pluginList.GetNextSelectedItem( pos );
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
		if ( plugin ) {
			switch ( m_runModeComboBox.GetCurSel() ) {
				case 0: plugin->setRunMode( rdoPlugin::prmNoAuto ); break;
				case 1: plugin->setRunMode( rdoPlugin::prmStudioStartUp ); break;
				case 2: plugin->setRunMode( rdoPlugin::prmModelStartUp ); break;
			}
			updateRunModeInGrid( plugin, index );
			updateStateInGrid( plugin, index );
			updateControls( plugin );
		}
	}
}

void RDOStudioOptionsPlugins::OnPluginRunModeButtonClicked() 
{
	POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
	if ( pos ) {
		int index = m_pluginList.GetNextSelectedItem( pos );
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
		if ( plugin ) {
			plugin->setRunMode( plugin->getDefaultRunMode() );
			updateRunModeInGrid( plugin, index );
			updateStateInGrid( plugin, index );
			updateControls( plugin );
		}
	}
}

void RDOStudioOptionsPlugins::OnPluginStart() 
{
	POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
	if ( pos ) {
		int index = m_pluginList.GetNextSelectedItem( pos );
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
		if ( plugin ) {
			plugin->setState( rdoPlugin::psActive );
			updateStateInGrid( plugin, index );
			updateControls( plugin );
		}
	}
}

void RDOStudioOptionsPlugins::OnPluginStop() 
{
	POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
	if ( pos ) {
		int index = m_pluginList.GetNextSelectedItem( pos );
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
		if ( plugin ) {
			plugin->setState( rdoPlugin::psStoped );
			updateStateInGrid( plugin, index );
			updateControls( plugin );
		}
	}
}

void RDOStudioOptionsPlugins::OnPluginRestoreStateCheckBoxClicked() 
{
	POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
	if ( pos ) {
		int index = m_pluginList.GetNextSelectedItem( pos );
		RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
		if ( plugin ) {
			plugin->setRestoreState( m_restoreStateCheckBox.GetCheck() ? true : false );
		}
	}
}

void RDOStudioOptionsPlugins::OnTimer( UINT nIDEvent )
{
	if ( nIDEvent == timer ) {
		int cnt = m_pluginList.GetItemCount();
		for ( int i = 0; i < cnt; i++ ) {
			RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( i ));
			if ( plugin ) {
				tstring state = rdo::format( plugin->getState() == rdoPlugin::psStoped ? IDS_PLUGIN_STATE_STOPED : IDS_PLUGIN_STATE_ACTIVE );
				if ( m_pluginList.GetItemText( i, 3 ).Compare( state.c_str() ) ) {
					updateStateInGrid( plugin, i );
				}
			}
		}
		POSITION pos = m_pluginList.GetFirstSelectedItemPosition();
		if ( pos ) {
			int index = m_pluginList.GetNextSelectedItem( pos );
			RDOStudioPlugin* plugin = reinterpret_cast<RDOStudioPlugin*>(m_pluginList.GetItemData( index ));
			if ( plugin ) {
				updateControls( plugin );
			}
		}
	}
	CPropertyPage::OnTimer( nIDEvent );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptions
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOptions, CPropertySheet)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

RDOStudioOptions::RDOStudioOptions():
	CPropertySheet(),
	general( NULL ),
	editor( NULL ),
	tabs( NULL ),
	styles( NULL ),
	plugins( NULL ),
	chart_need_delete( true )
{
	SetTitle( rdo::format( ID_OPTIONS ).c_str() );

	style_editor.init();
	style_build.init();
	style_debug.init();
	style_trace.init();
	style_results.init();
	style_find.init();
	style_chart.init();
	style_frame.init();

	style_editor  = studioApp.m_pMainFrame->style_editor;
	style_build   = studioApp.m_pMainFrame->style_build;
	style_debug   = studioApp.m_pMainFrame->style_debug;
	style_trace   = studioApp.m_pMainFrame->style_trace;
	style_results = studioApp.m_pMainFrame->style_results;
	style_find    = studioApp.m_pMainFrame->style_find;
	style_chart   = studioApp.m_pMainFrame->style_chart;
	style_frame   = studioApp.m_pMainFrame->style_frame;

	general = new RDOStudioOptionsGeneral( *this );
	editor  = new RDOStudioOptionsEditor( *this );
	tabs    = new RDOStudioOptionsTabs( *this );
	styles  = new RDOStudioOptionsColorsStyles( *this );
	plugins = new RDOStudioOptionsPlugins( *this );

	preview_chart_doc = new RDOStudioChartDoc( true );
	preview_chart     = new RDOStudioChartView( true );

	AddPage( general );
	AddPage( editor );
	AddPage( tabs );
	AddPage( styles );
	AddPage( plugins );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioOptions::~RDOStudioOptions()
{
	//dont call destructors for preview_chart & preview_chart_doc
	//if the property page that contains them was showed (chart_need_delete == false)
	//because framework kills them itself
	if ( chart_need_delete ) {
		delete preview_chart_doc; preview_chart_doc = NULL;
		delete preview_chart;     preview_chart = NULL;
	}
	if ( general )  { delete general; general = NULL; }
	if ( editor )   { delete editor; editor = NULL; }
	if ( tabs )     { delete tabs;   tabs = NULL; }
	if ( styles )   { delete styles; styles = NULL; }
	if ( plugins )  { delete plugins; plugins = NULL; }
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
	if ( preview_chart->GetSafeHwnd() ) {
		preview_chart->setStyle( &style_chart );
	}
	if ( preview_frame.GetSafeHwnd() ) {
		preview_frame.setStyle( &style_frame );
	}
}

void RDOStudioOptions::apply() const
{
	studioApp.m_pMainFrame->style_editor  = style_editor;
	studioApp.m_pMainFrame->style_build   = style_build;
	studioApp.m_pMainFrame->style_debug   = style_debug;
	studioApp.m_pMainFrame->style_trace   = style_trace;
	studioApp.m_pMainFrame->style_results = style_results;
	studioApp.m_pMainFrame->style_find    = style_find;
	studioApp.m_pMainFrame->style_chart   = style_chart;
	studioApp.m_pMainFrame->style_frame   = style_frame;
	studioApp.m_pMainFrame->updateAllStyles();
	studioApp.setFileAssociationSetup( general->m_setup ? true : false );
	studioApp.setFileAssociationCheckInFuture( general->m_checkInFuture ? true : false );
	studioApp.setOpenLastProject( general->m_openLastProject ? true : false );
	studioApp.setShowCaptionFullName( general->m_showFullName ? true : false );
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
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	CPropertyPage* page = GetActivePage( );
	if ( page == general ) {
		filename += "::/html/work_options_general.htm";
	} else if ( page == editor ) {
		filename += "::/html/work_options_editor.htm";
	} else if ( page == tabs ) {
		filename += "::/html/work_options_tabs.htm";
	} else if ( page == styles ) {
		filename += "::/html/work_options_styles_and_color.htm";
	} else if ( page == plugins ) {
		filename += "::/html/work_options.htm";
	}
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

BOOL RDOStudioOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return TRUE;

	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
		return ::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}

void RDOStudioOptionsColorsStyles::OnCommentGroupCheck()
{
	STYLEObject* obj = getCurrentObject();
	if ( obj && &obj->commentfold != &null_commentfold ) {
		obj->commentfold = m_commentGroupButton.GetCheck() ? true : false;
		OnUpdateModify();
	}
}

BOOL RDOStudioOptions::ContinueModal()
{
	if ( !::IsWindow(m_hWnd) ) return false;
	return CPropertySheet::ContinueModal();
}
