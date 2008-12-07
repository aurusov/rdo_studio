#include "stdafx.h"
#include "rdotraceroptions.h"
#include "rdotracerapp.h"
#include "rdotracermainfrm.h"
#include "../htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;
using namespace rdoTracerLog;
using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerOptionsColorsStyles
// ----------------------------------------------------------------------------
COLORREF RDOTracerOptionsColorsStyles::null_fg_color = RGB( 0x00, 0x00, 0x00 );
COLORREF RDOTracerOptionsColorsStyles::null_bg_color = RGB( 0xFF, 0xFF, 0xFF );

BEGIN_MESSAGE_MAP(RDOTracerOptionsColorsStyles, CPropertyPage)
	//{{AFX_MSG_MAP(RDOTracerOptionsColorsStyles)
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
	ON_CBN_SELCHANGE(IDC_THEME_COMBO, OnThemeChanged)
	ON_EN_CHANGE(IDC_VERTBORDER_EDIT, OnUpdateModify)
	ON_EN_CHANGE(IDC_HORZBORDER_EDIT, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_TITLE_FONTSIZE_COMBO, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_LEGEND_FONTSIZE_COMBO, OnUpdateModify)
	ON_EN_CHANGE(IDC_TICKWIDTH_EDIT, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTracerOptionsColorsStyles::RDOTracerOptionsColorsStyles( RDOTracerOptions& _sheet ):
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
	//{{AFX_DATA_INIT(RDOTracerOptionsColorsStyles)
	//}}AFX_DATA_INIT

	STYLEObject* object;
	object = new STYLEObject( STYLEObject::all, all_font_name, all_font_size );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_ALLWINDOW ), null_font_style, all_fg_color, all_bg_color ) );
	objects.push_back( object );

	RDOTracerLogTheme* trace_theme = static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme);
	object = new STYLEObject( STYLEObject::trace, sheet->style_trace.font->name, sheet->style_trace.font->size );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG ), trace_theme->style, trace_theme->defaultColor.foregroundColor, trace_theme->defaultColor.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_ES ), trace_theme->style, trace_theme->es.foregroundColor, trace_theme->es.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_EB ), trace_theme->style, trace_theme->eb.foregroundColor, trace_theme->eb.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_EF ), trace_theme->style, trace_theme->ef.foregroundColor, trace_theme->ef.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_EI ), trace_theme->style, trace_theme->ei.foregroundColor, trace_theme->ei.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_ER ), trace_theme->style, trace_theme->er.foregroundColor, trace_theme->er.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_RC ), trace_theme->style, trace_theme->rc.foregroundColor, trace_theme->rc.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_RE ), trace_theme->style, trace_theme->re.foregroundColor, trace_theme->re.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_RK ), trace_theme->style, trace_theme->rk.foregroundColor, trace_theme->rk.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_V ),  trace_theme->style, trace_theme->v.foregroundColor,  trace_theme->v.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_STATUS ), trace_theme->style, trace_theme->s.foregroundColor, trace_theme->s.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_DPS ), trace_theme->style, trace_theme->dps.foregroundColor, trace_theme->dps.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SB ),  trace_theme->style, trace_theme->sb.foregroundColor,  trace_theme->sb.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SO ),  trace_theme->style, trace_theme->so.foregroundColor,  trace_theme->so.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_STN ), trace_theme->style, trace_theme->stn.foregroundColor, trace_theme->stn.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_STD ), trace_theme->style, trace_theme->std.foregroundColor, trace_theme->std.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_STR ), trace_theme->style, trace_theme->str.foregroundColor, trace_theme->str.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SRC ), trace_theme->style, trace_theme->src.foregroundColor, trace_theme->src.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SRE ), trace_theme->style, trace_theme->sre.foregroundColor, trace_theme->sre.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SRK ), trace_theme->style, trace_theme->srk.foregroundColor, trace_theme->srk.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SD ),  trace_theme->style, trace_theme->sd.foregroundColor,  trace_theme->sd.backgroundColor  ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SES ), trace_theme->style, trace_theme->ses.foregroundColor, trace_theme->ses.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SEN ), trace_theme->style, trace_theme->sen.foregroundColor, trace_theme->sen.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SEM ), trace_theme->style, trace_theme->sem.foregroundColor, trace_theme->sem.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SEF ), trace_theme->style, trace_theme->sef.foregroundColor, trace_theme->sef.backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_LOG_SEU ), trace_theme->style, trace_theme->seu.foregroundColor, trace_theme->seu.backgroundColor ) );
	objects.push_back( object );

	RDOStudioChartViewTheme* chart_theme = static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme);
	object = new STYLEObject( STYLEObject::chart, sheet->style_chart.font->name, sheet->style_chart.font->size, false );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART ), chart_theme->defaultStyle, chart_theme->defaultColor, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART_AXIS ), chart_theme->defaultStyle, chart_theme->axisFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART_TITLE ), chart_theme->titleStyle, chart_theme->titleFGColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART_LEGEND ), chart_theme->legendStyle, chart_theme->legendFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART_CHART ), null_font_style, null_fg_color, chart_theme->chartBgColor ) );
	object->properties.push_back( new STYLEProperty( object, format( IDS_COLORSTYLE_CHART_TIME ), null_font_style, null_fg_color, chart_theme->timeBgColor ) );
	objects.push_back( object );

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOTracerOptionsColorsStyles::~RDOTracerOptionsColorsStyles()
{
	list< STYLEObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		delete *it++;
	};
}

void RDOTracerOptionsColorsStyles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOTracerOptionsColorsStyles)
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

int CALLBACK RDOTracerOptionsColorsStyles::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam )
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

BOOL RDOTracerOptionsColorsStyles::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = static_cast<CComboBox*>(GetDlgItem( IDC_THEME_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	sheet->preview_trace.Create( NULL, NULL, WS_CHILD, CRect( 0, 0, 444, 223 ), this, 0 );
	sheet->preview_trace.setStyle( &sheet->style_trace );
	sheet->preview_trace.setFocusOnly( true );
	sheet->preview_trace.setShowMenu( false );
	sheet->preview_trace.setText( format( IDS_COLORSTYLE_LOG_SAMPLE ) );
	sheet->preview_trace.selectLine( 0 );

	sheet->preview_chart_doc->SetTitle( format( IDS_COLORSTYLE_CHART_SAMPLE1 ).c_str() );
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
	sheet->preview_serie.setTitle( format( IDS_COLORSTYLE_CHART_SAMPLE2 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 0 ), 2, 0 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 1 ), 1, 1 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 2 ), 0, 4 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 3 ), 3, 3 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 4 ), 1, 2 ) );
	sheet->preview_serie.addValue( new RDOTracerValue( &sheet->preview_times.at( 5 ), 0, 3 ) );
	sheet->preview_chart_doc->addSerie( &sheet->preview_serie );
	sheet->chart_need_delete = false;

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

	sheet->preview_trace.MoveWindow( rectEdit );
	sheet->preview_chart->MoveWindow( rectEdit );

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
	lf.lfCharSet = static_cast<unsigned char>(sheet->style_trace.font->characterSet);
	lf.lfFaceName[0] = '\0';
	CDC* dc = GetDC();
	::EnumFontFamiliesEx( dc->m_hDC, &lf, reinterpret_cast<FONTENUMPROC>(RDOTracerOptionsColorsStyles::EnumFontFamExProc), reinterpret_cast<LPARAM>(&fonts), 0 );
	ReleaseDC( dc );
	isCurrentFixed = false;
	loadFontsIntoCombo( true );

	int vertBorder = sheet->style_trace.borders->vertBorder;
	int horzBorder = sheet->style_trace.borders->horzBorder;
	int title_font_size = sheet->style_chart.fonts_ticks->titleFontSize;
	int legent_font_size = sheet->style_chart.fonts_ticks->legendFontSize;
	int tick_width = sheet->style_chart.fonts_ticks->tickWidth;
	m_vertBorder.SetWindowText( format( "%d", vertBorder ).c_str() );
	m_horzBorder.SetWindowText( format( "%d", horzBorder ).c_str() );
	m_vertBorder.SetLimitText( 2 );
	m_horzBorder.SetLimitText( 2 );

	int index = m_title_fontSizeCombo.FindStringExact( -1, format( "%d", title_font_size ).c_str() );
	if ( index != CB_ERR ) {
		m_title_fontSizeCombo.SetCurSel( index );
	} else {
		m_title_fontSizeCombo.SetCurSel( -1 );
	}
	index = m_leg_fontSizeCombo.FindStringExact( -1, format( "%d", legent_font_size ).c_str() );
	if ( index != CB_ERR ) {
		m_leg_fontSizeCombo.SetCurSel( index );
	} else {
		m_leg_fontSizeCombo.SetCurSel( -1 );
	}
	m_tickWidth.SetWindowText( format( "%d", tick_width ).c_str() );
	m_tickWidth.SetLimitText( 2 );
	
	setPreviewAsCombo( STYLEObject::trace );

	updatePropOfAllObject();
	m_styleItem.SelectItem( m_styleItem.GetRootItem() );

	return true;
}

void RDOTracerOptionsColorsStyles::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOTracerOptionsColorsStyles::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOTracerOptionsColorsStyles::OnStyleItemChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	updateStyleItem();

	*pResult = 0;
}

void RDOTracerOptionsColorsStyles::updateStyleItem()
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
		m_fontStyleBold.SetCheck( prop->font_style & RDOStyleFont::BOLD );
		m_fontStyleItalic.SetCheck( prop->font_style & RDOStyleFont::ITALIC );
		m_fontStyleUnderline.SetCheck( prop->font_style & RDOStyleFont::UNDERLINE );

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

		// Update borders
		bool flag_border = type == STYLEObject::trace;
		m_vertBorder.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_vertBorderStatic.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_horzBorder.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );
		m_horzBorderStatic.ShowWindow( flag_border ? SW_SHOW : SW_HIDE );

		// Update fonts and ticks
		bool flag_fonts_ticks = type == STYLEObject::chart;
		m_title_fontSizeStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_leg_fontSizeStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_leg_fontSizeCombo.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_title_fontSizeCombo.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_tickWidth.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );
		m_tickWidthStatic.ShowWindow( flag_fonts_ticks ? SW_SHOW : SW_HIDE );

		// Update preview
		bool flag_preview = type == STYLEObject::all;
		m_previewAs.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		m_previewAsStatic.ShowWindow( flag_preview ? SW_SHOW : SW_HIDE );
		setPreviewAsCombo( type );

		// Update theme
		m_theme.ResetContent();
		m_theme.AddString( format( IDS_COLORSTYLE_EDITOR_THEME_CURRENT ).c_str() );
		m_theme.AddString( format( IDS_COLORSTYLE_EDITOR_THEME_DEFAULT ).c_str() );
		switch ( type ) {
			case STYLEObject::all: {
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

void RDOTracerOptionsColorsStyles::updateTheme()
{
	switch ( getCurrentObjectType() ) {
		case STYLEObject::all: {
			if ( *static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) == RDOTracerLogTheme::getDefaultTheme() &&
				 *static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) == RDOStudioChartViewTheme::getDefaultTheme() &&
				 *sheet->style_trace.font == RDOStyleFont::getTracerLogFont() &&
				 *sheet->style_chart.font == RDOStyleFont::getChartViewFont() ) {
				m_theme.SetCurSel( 1 );
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
		default: break;
	}
}

void RDOTracerOptionsColorsStyles::OnThemeChanged()
{
	int index = m_theme.GetCurSel();
	if ( index != CB_ERR ) {
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				if ( index == 1 ) {
					*static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) = RDOTracerLogTheme::getDefaultTheme();
					*static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) = RDOStudioChartViewTheme::getDefaultTheme();
					*sheet->style_trace.font   = RDOStyleFont::getTracerLogFont();
					*sheet->style_chart.font   = RDOStyleFont::getChartViewFont();
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
			case STYLEObject::chart: {
				RDOStudioChartViewTheme* theme = static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme);
				switch ( index ) {
					case 1: *theme = RDOStudioChartViewTheme::getDefaultTheme(); *sheet->style_trace.font = RDOStyleFont::getChartViewFont(); break;
				}
				break;
			}
		}
		updatePropOfAllObject();
		updateStyleItem();
		OnUpdateModify();
	}
}

void RDOTracerOptionsColorsStyles::OnFontNameChanged()
{
	int index = m_fontName.GetCurSel();
	if ( index != CB_ERR ) {
		CString str;
		m_fontName.GetLBText( index, str );
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				all_font_name = str;
				sheet->style_trace.font->name   = all_font_name;
				sheet->style_chart.font->name   = all_font_name;
				break;
			}
			case STYLEObject::trace: {
				sheet->style_trace.font->name = str;
				break;
			}
			case STYLEObject::chart: {
				sheet->style_chart.font->name = str;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOTracerOptionsColorsStyles::OnFontSizeChanged()
{
	int index = m_fontSize.GetCurSel();
	if ( index != CB_ERR ) {
		CString str;
		m_fontSize.GetLBText( index, str );
		int size = atoi( str );
		switch ( getCurrentObjectType() ) {
			case STYLEObject::all: {
				all_font_size = size;
				sheet->style_trace.font->size   = all_font_size;
				sheet->style_chart.font->size   = all_font_size;
				break;
			}
			case STYLEObject::trace: {
				sheet->style_trace.font->size = size;
				break;
			}
			case STYLEObject::chart: {
				sheet->style_chart.font->size = size;
				break;
			}
			default: break;
		}
		OnUpdateModify();
	}
}

void RDOTracerOptionsColorsStyles::OnFontStyleBoldChanged()
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

void RDOTracerOptionsColorsStyles::OnFontStyleItalicChanged()
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

void RDOTracerOptionsColorsStyles::OnFontStyleUnderlineChanged()
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

void RDOTracerOptionsColorsStyles::OnFgColorChanged()
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

void RDOTracerOptionsColorsStyles::OnBgColorChanged()
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

void RDOTracerOptionsColorsStyles::OnFgColorClicked()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorChanged();
	}
}

void RDOTracerOptionsColorsStyles::OnBgColorClicked()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorChanged();
	}
}

void RDOTracerOptionsColorsStyles::OnPreviewAsChanged()
{
	int index = m_previewAs.GetCurSel();
	if ( index != CB_ERR ) {
		if ( index == 0 ) {
			setPreviewAsCombo( STYLEObject::trace );
		} else if ( index == 1 ) {
			setPreviewAsCombo( STYLEObject::chart );
		}
	}
}

void RDOTracerOptionsColorsStyles::OnUpdateModify()
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
		m_tickWidth.SetWindowText( format( "%d", sheet->style_chart.fonts_ticks->tickWidth ).c_str() );
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

	SetModified( *sheet->style_trace.font   != *rdoTracerApp.mainFrame->style_trace.font ||
	             *sheet->style_chart.font   != *rdoTracerApp.mainFrame->style_chart.font ||
	             *static_cast<RDOTracerLogTheme*>(sheet->style_trace.theme) != *static_cast<RDOTracerLogTheme*>(rdoTracerApp.mainFrame->style_trace.theme) ||
				 *static_cast<RDOStudioChartViewTheme*>(sheet->style_chart.theme) != *static_cast<RDOStudioChartViewTheme*>(rdoTracerApp.mainFrame->style_chart.theme) ||
	             *sheet->style_trace.borders     != *rdoTracerApp.mainFrame->style_trace.borders || 
				 *sheet->style_chart.fonts_ticks != *rdoTracerApp.mainFrame->style_chart.fonts_ticks );
}

void RDOTracerOptionsColorsStyles::loadFontsIntoCombo( bool fixed )
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

RDOTracerOptionsColorsStyles::STYLEProperty* RDOTracerOptionsColorsStyles::getCurrentProperty() const
{
	HTREEITEM item = m_styleItem.GetSelectedItem();
	return item ? reinterpret_cast<RDOTracerOptionsColorsStyles::STYLEProperty*>(m_styleItem.GetItemData( item )) : NULL;
}

RDOTracerOptionsColorsStyles::STYLEObject* RDOTracerOptionsColorsStyles::getCurrentObject() const
{
	STYLEProperty* prop = getCurrentProperty();
	return prop ? prop->object : NULL;
}

RDOTracerOptionsColorsStyles::STYLEObject::Type RDOTracerOptionsColorsStyles::getCurrentObjectType() const
{
	STYLEObject* obj = getCurrentObject();
	return obj ? obj->type : STYLEObject::none;
}

void RDOTracerOptionsColorsStyles::setPreviewAsCombo( STYLEObject::Type type )
{
	if ( previewAs != type && type >= STYLEObject::trace ) {
		previewAs = type;
		sheet->preview_trace.ShowWindow( SW_HIDE );
		sheet->preview_chart->ShowWindow( SW_HIDE );
		switch ( previewAs ) {
			case STYLEObject::trace: {
				m_previewAs.SetCurSel( 0 );
				sheet->preview_trace.ShowWindow( SW_SHOW );
				break;
			}
			case STYLEObject::chart: {
				m_previewAs.SetCurSel( 1 );
				sheet->preview_chart->ShowWindow( SW_SHOW );
				break;
			}
			default: break;
		}
	}
}

void RDOTracerOptionsColorsStyles::updatePropOfAllObject()
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
// ---------- RDOTracerOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOTracerOptions, CPropertySheet)
	//{{AFX_MSG_MAP(RDOTracerOptions)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTracerOptions::RDOTracerOptions():
	CPropertySheet(),
	styles( NULL ),
	chart_need_delete( true )
{
	SetTitle( format( ID_OPTIONS ).c_str() );

	style_trace.init();
	style_chart.init();

	style_trace   = rdoTracerApp.mainFrame->style_trace;
	style_chart   = rdoTracerApp.mainFrame->style_chart;

	styles  = new RDOTracerOptionsColorsStyles( *this );

	preview_chart_doc = new RDOStudioChartDoc( true );
	preview_chart     = new RDOStudioChartView( true );

	AddPage( styles );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOTracerOptions::~RDOTracerOptions()
{
	//dont call destructors for preview_chart & preview_chart_doc
	//if the property page that contains them was showed (chart_need_delete == false)
	//because framework kills them itself
	if ( chart_need_delete ) {
		delete preview_chart_doc; preview_chart_doc = NULL;
		delete preview_chart;     preview_chart = NULL;
	}
	if ( styles )   { delete styles; styles = NULL; }
}

void RDOTracerOptions::updateStyles()
{
	if ( preview_trace.GetSafeHwnd() ) {
		preview_trace.setStyle( &style_trace );
	}
	if ( preview_chart->GetSafeHwnd() ) {
		preview_chart->setStyle( &style_chart );
	}
}

void RDOTracerOptions::apply() const
{
	rdoTracerApp.mainFrame->style_trace   = style_trace;
	rdoTracerApp.mainFrame->style_chart   = style_chart;
	rdoTracerApp.mainFrame->updateAllStyles();
}

int CALLBACK RDOTracerOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM /*lParam*/)
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

void RDOTracerOptions::onHelpButton()
{
	string filename = rdoTracerApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	CPropertyPage* page = GetActivePage( );
	if ( page == styles ) {
		filename += "::/html/work_options.htm#styles";
	}
	HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

BOOL RDOTracerOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	string filename = rdoTracerApp.getFullHelpFileName();
	if ( filename.empty() ) return TRUE;

	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
		return HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
