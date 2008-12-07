#include "stdafx.h"
//#include "rdotraceroptions.h"
//#include "../resource.h"
//#include "../htmlhelp.h"
//#include "rdotracermainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/*
// ----------------------------------------------------------------------------
// ---------- RDOTracerLogOptions
// ----------------------------------------------------------------------------
enum RDOLogStyleItem {
	RDOLSI_ES = 0,
	RDOLSI_EB,
	RDOLSI_EF,
	RDOLSI_EI,
	RDOLSI_ER,
	RDOLSI_RC,
	RDOLSI_RE,
	RDOLSI_RK,
	RDOLSI_V,
	RDOLSI_S,
	RDOLSI_DPS,
	RDOLSI_SB,
	RDOLSI_SO,
	RDOLSI_STN,
	RDOLSI_STD,
	RDOLSI_STR,
	RDOLSI_SRC,
	RDOLSI_SRE,
	RDOLSI_SRK,
	RDOLSI_SD,
	RDOLSI_SES,
	RDOLSI_SEN,
	RDOLSI_SEM,
	RDOLSI_SEF,
	RDOLSI_SEU
};

BEGIN_MESSAGE_MAP( RDOTracerLogOptions, CPropertyPage )
	
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

	ON_EN_CHANGE( IDC_VERTBORDER_EDIT  , OnUpdateModify )
	ON_EN_CHANGE( IDC_HORZBORDER_EDIT  , OnUpdateModify )

	ON_BN_CLICKED( IDC_LOG_RESET_BUTTON  , OnResetBClick )
	ON_BN_CLICKED( IDC_LOG_DEFAULT_BUTTON, OnDefaultBClick )

	ON_MESSAGE( WM_LOGSELCHANGE, onLogSelChange )

END_MESSAGE_MAP()

RDOTracerLogOptions::RDOTracerLogOptions( RDOTracerOptions& _sheet ):
	CPropertyPage( IDD_OPTIONS_LOG_DIALOG ),
	sheet( &_sheet )
{
		m_psp.dwFlags |= PSP_HASHELP;
}

void RDOTracerLogOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_FGCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_BGCOLOR_COMBO, bgColorCB );

	DDX_Text(  pDX, IDC_VERTBORDER_EDIT, sheet->logStyle.vertBorder );
	DDX_Text(  pDX, IDC_HORZBORDER_EDIT, sheet->logStyle.horzBorder );
}

int CALLBACK RDOTracerLogOptions::EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam )
{
	if ( lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH ) {
		CStringList* tmpFontList = (CStringList*)lParam;
		if ( !tmpFontList->Find( (char*)lpelfe->elfLogFont.lfFaceName ) ) tmpFontList->AddTail( lpelfe->elfLogFont.lfFaceName );
	}
	return 1;
}

BOOL RDOTracerLogOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	log.Create( NULL, NULL, NULL, CRect( 50, 60, 150, 160 ), this, -1 );
	log.setStyle( sheet->logStyle );
	log.setFocusOnly( true );
	log.setShowMenu( false );
	CString s;
	s.LoadString( IDS_LOG_SAMPLE );
	while ( !s.IsEmpty() ) {
		int found = s.Find( "\n" );
		if ( found != -1 ) {
			CString str = s.Left( found );
			s.Delete( 0, found + 1 );
			log.addStringToLog( str );
		}
	}
	
	currentStyle = sheet->logStyle;
	defaultStyle = RDOTracerLogStyle::getDefaultStyle();

	CListBox* styleLB = (CListBox*)GetDlgItem( IDC_STYLE_LIST );
	if ( styleLB ) {
		CString s;
		CSize sz;
		int width = 0;
		CDC* dc = styleLB->GetDC();
		for ( int i = IDS_LOGSTYLE_ES; i <= IDS_LOGSTYLE_SEU; i++ ) {
			s.LoadString( i );
			styleLB->AddString( s );
			sz = dc->GetTextExtent( s );
			if ( sz.cx > width )
				width = sz.cx;
		}
		styleLB->ReleaseDC( dc );
		styleLB->SetCurSel( 0 );
		styleLB->SetHorizontalExtent( width );
	}

	LOGFONT lf;
	memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfCharSet = sheet->logStyle.font.characterSet;

	fontList.RemoveAll();
	::EnumFontFamiliesEx( GetDC()->m_hDC, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&fontList, 0 );

	CComboBox* fontNameCB = (CComboBox*)GetDlgItem( IDC_FONTNAME_COMBO );
	if ( fontNameCB ) {
		POSITION pos;
		for( pos = fontList.GetHeadPosition(); pos != NULL; )
			fontNameCB->AddString( fontList.GetNext(pos) );

	}

	updateFont();
	updateButtons();

	OnStyleLBChanged();
	CRect rect;
	GetClientRect( &rect );

	if ( styleLB ) {
		CRect r;
		GetClientRect( r );

		CRect rectColorCB;
		bgColorCB.GetWindowRect( rectColorCB );
		ScreenToClient( rectColorCB );

		CRect rectStyleCB;
		styleLB->GetWindowRect( rectStyleCB );
		ScreenToClient( rectStyleCB );
		rectStyleCB.bottom = r.bottom;
		styleLB->MoveWindow( &rectStyleCB );

		CRect rectLog;
		rectLog.left   = rectStyleCB.right + 5;
		rectLog.right  = r.right - (rectStyleCB.left - r.left);
		rectLog.top    = rectColorCB.bottom + 10;
		rectLog.bottom = rectStyleCB.bottom;

		log.MoveWindow( rectLog );
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

	((CEdit*)GetDlgItem( IDC_VERTBORDER_EDIT ))->SetLimitText( 2 );
	((CEdit*)GetDlgItem( IDC_HORZBORDER_EDIT ))->SetLimitText( 2 );

	return true;
}

void RDOTracerLogOptions::OnOK()
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOTracerLogOptions::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( ((LPNMHDR) lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify( wParam, lParam, pResult );
}

void RDOTracerLogOptions::OnStyleLBChanged()
{
	RDOTracerLogStyle* style = &sheet->logStyle;

	COLORREF fg;
	COLORREF bg;
	RDOFontStyle fontStyle = style->font.style;
	bool enableFg = true;
	bool enableBg = true;
	bool enableFS = true;
	bool showFg   = true;
	bool showBg   = true;
	CString str;
	
	int item = ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel();
	switch ( item ) {
		case RDOLSI_ES:
			str = "ES";
			break;
		case RDOLSI_EB:
			str = "EB";
			break;
		case RDOLSI_EF:
			str = "EF";
			break;
		case RDOLSI_EI:
			str = "EI";
			break;
		case RDOLSI_ER:
			str = "ER";
			break;
		case RDOLSI_RC:
			str = "RC";
			break;
		case RDOLSI_RE:
			str = "RE";
			break;
		case RDOLSI_RK:
			str = "RK";
			break;
		case RDOLSI_V:
			str = "V";
			break;
		case RDOLSI_S:
			str = "$Status";
			break;
		case RDOLSI_DPS:
			str = "DPS";
			break;
		case RDOLSI_SB:
			str = "SB";
			break;
		case RDOLSI_SO:
			str = "SO";
			break;
		case RDOLSI_STN:
			str = "STN";
			break;
		case RDOLSI_STD:
			str = "STD";
			break;
		case RDOLSI_STR:
			str = "STR";
			break;
		case RDOLSI_SRC:
			str = "SRC";
			break;
		case RDOLSI_SRE:
			str = "SRE";
			break;
		case RDOLSI_SRK:
			str = "SRK";
			break;
		case RDOLSI_SD:
			str = "SD";
			break;
		case RDOLSI_SES:
			str = "SES";
			break;
		case RDOLSI_SEN:
			str = "SEN";
			break;
		case RDOLSI_SEM:
			str = "SEM";
			break;
		case RDOLSI_SEF:
			str = "SEF";
			break;
		case RDOLSI_SEU:
			str = "SEU";
			break;
	}
	
	style->getItemColors( str, fg, bg );

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

	log.selectLine( item );
}

LRESULT RDOTracerLogOptions::onLogSelChange( WPARAM wParam, LPARAM lParam )
{
	if ( lParam != -1 ) {
		CListBox* list = (CListBox*)GetDlgItem( IDC_STYLE_LIST );
		list->SetCurSel( (int)lParam );
		OnStyleLBChanged();
	}
	return 0;
}

void RDOTracerLogOptions::OnFontNameCBChanged()
{
	CComboBox* fontNameCB = (CComboBox*)GetDlgItem( IDC_FONTNAME_COMBO );
	if ( fontNameCB ) {
		int index = fontNameCB->GetCurSel();
		if ( index != CB_ERR ) {
			fontNameCB->GetLBText( index, sheet->logStyle.font.name );
		}
	}
	updateLog();
}

void RDOTracerLogOptions::OnFontSizeCBChanged()
{
	CComboBox* fontSizeCB = (CComboBox*)GetDlgItem( IDC_FONTSIZE_COMBO );
	if ( fontSizeCB ) {
		int index = fontSizeCB->GetCurSel();
		if ( index != CB_ERR ) {
			CString s;
			fontSizeCB->GetLBText( index, s );
			sheet->logStyle.font.size = strtol( s, NULL, 10 );
		}
	}
	updateLog();
}

void RDOTracerLogOptions::OnFontBoldBClick()
{
	bool bold = ((CButton*)GetDlgItem( IDC_FONTBOLD_CHECK ))->GetCheck() ? true : false;
	RDOTracerLogStyle* style = &sheet->logStyle;
	RDOFontStyle fontStyle;
	
	if ( bold ) {
		fontStyle = RDOFS_BOLD;
		style->font.style = (RDOFontStyle)(style->font.style | fontStyle);
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_BOLD;
		style->font.style = (RDOFontStyle)(style->font.style & fontStyle);
	}

	updateLog();
}

void RDOTracerLogOptions::OnFontItalicBClick()
{
	bool italic = ((CButton*)GetDlgItem( IDC_FONTITALIC_CHECK ))->GetCheck() ? true : false;
	RDOTracerLogStyle* style = &sheet->logStyle;
	RDOFontStyle fontStyle;

	if ( italic ) {
		fontStyle = RDOFS_ITALIC;
		style->font.style = (RDOFontStyle)(style->font.style | fontStyle);
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_ITALIC;
		style->font.style = (RDOFontStyle)(style->font.style & fontStyle);
	}

	updateLog();
}

void RDOTracerLogOptions::OnFontUnderlineBClick()
{
	bool underline = ((CButton*)GetDlgItem( IDC_FONTUNDERLINE_CHECK ))->GetCheck() ? true : false;
	RDOTracerLogStyle* style = &sheet->logStyle;
	RDOFontStyle fontStyle;

	if ( underline ) {
		fontStyle = RDOFS_UNDERLINE;
		style->font.style = (RDOFontStyle)(style->font.style | fontStyle);
	} else {
		fontStyle = (RDOFontStyle)~RDOFS_UNDERLINE;
		style->font.style = (RDOFontStyle)(style->font.style & fontStyle);
	}
	updateLog();
}

void RDOTracerLogOptions::OnFgColorCBChanged()
{
	COLORREF color = fgColorCB.getCurrentColor();
	RDOTracerLogStyle* style = &sheet->logStyle;

	switch ( ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel() ) {
		case RDOLSI_ES  : style->es.foregroundColor  = color; break;
		case RDOLSI_EB  : style->eb.foregroundColor  = color; break;
		case RDOLSI_EF  : style->ef.foregroundColor  = color; break;
		case RDOLSI_EI  : style->ei.foregroundColor  = color; break;
		case RDOLSI_ER  : style->er.foregroundColor  = color; break;
		case RDOLSI_RC  : style->rc.foregroundColor  = color; break;
		case RDOLSI_RE  : style->re.foregroundColor  = color; break;
		case RDOLSI_RK  : style->rk.foregroundColor  = color; break;
		case RDOLSI_V   : style->v.foregroundColor   = color; break;
		case RDOLSI_S   : style->s.foregroundColor   = color; break;
		case RDOLSI_DPS : style->dps.foregroundColor = color; break;
		case RDOLSI_SB  : style->sb.foregroundColor  = color; break;
		case RDOLSI_SO  : style->so.foregroundColor  = color; break;
		case RDOLSI_STN : style->stn.foregroundColor = color; break;
		case RDOLSI_STD : style->std.foregroundColor = color; break;
		case RDOLSI_STR : style->str.foregroundColor = color; break;
		case RDOLSI_SRC : style->src.foregroundColor = color; break;
		case RDOLSI_SRE : style->sre.foregroundColor = color; break;
		case RDOLSI_SRK : style->srk.foregroundColor = color; break;
		case RDOLSI_SD  : style->sd.foregroundColor  = color; break;
		case RDOLSI_SES : style->ses.foregroundColor = color; break;
		case RDOLSI_SEN : style->sen.foregroundColor = color; break;
		case RDOLSI_SEM : style->sem.foregroundColor = color; break;
		case RDOLSI_SEF : style->sef.foregroundColor = color; break;
		case RDOLSI_SEU : style->seu.foregroundColor = color; break;
	}
	updateLog();
}

void RDOTracerLogOptions::OnBgColorCBChanged()
{
	COLORREF color = bgColorCB.getCurrentColor();
	RDOTracerLogStyle* style = &sheet->logStyle;
	
	switch ( ((CListBox*)GetDlgItem( IDC_STYLE_LIST ))->GetCurSel() ) {
		case RDOLSI_ES  : style->es.backgroundColor  = color; break;
		case RDOLSI_EB  : style->eb.backgroundColor  = color; break;
		case RDOLSI_EF  : style->ef.backgroundColor  = color; break;
		case RDOLSI_EI  : style->ei.backgroundColor  = color; break;
		case RDOLSI_ER  : style->er.backgroundColor  = color; break;
		case RDOLSI_RC  : style->rc.backgroundColor  = color; break;
		case RDOLSI_RE  : style->re.backgroundColor  = color; break;
		case RDOLSI_RK  : style->rk.backgroundColor  = color; break;
		case RDOLSI_V   : style->v.backgroundColor   = color; break;
		case RDOLSI_S   : style->s.backgroundColor   = color; break;
		case RDOLSI_DPS : style->dps.backgroundColor = color; break;
		case RDOLSI_SB  : style->sb.backgroundColor  = color; break;
		case RDOLSI_SO  : style->so.backgroundColor  = color; break;
		case RDOLSI_STN : style->stn.backgroundColor = color; break;
		case RDOLSI_STD : style->std.backgroundColor = color; break;
		case RDOLSI_STR : style->str.backgroundColor = color; break;
		case RDOLSI_SRC : style->src.backgroundColor = color; break;
		case RDOLSI_SRE : style->sre.backgroundColor = color; break;
		case RDOLSI_SRK : style->srk.backgroundColor = color; break;
		case RDOLSI_SD  : style->sd.backgroundColor  = color; break;
		case RDOLSI_SES : style->ses.backgroundColor = color; break;
		case RDOLSI_SEN : style->sen.backgroundColor = color; break;
		case RDOLSI_SEM : style->sem.backgroundColor = color; break;
		case RDOLSI_SEF : style->sef.backgroundColor = color; break;
		case RDOLSI_SEU : style->seu.backgroundColor = color; break;
	}
	updateLog();
}

void RDOTracerLogOptions::OnFgColorBClick()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorCBChanged();
	}
}

void RDOTracerLogOptions::OnBgColorBClick()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorCBChanged();
	}
}

afx_msg void RDOTracerLogOptions::OnResetBClick()
{
	sheet->logStyle = currentStyle;
	UpdateData( false );
	OnStyleLBChanged();
	updateFont();
	updateLog();
	OnUpdateModify();
}

afx_msg void RDOTracerLogOptions::OnDefaultBClick()
{
	sheet->logStyle = defaultStyle;
	UpdateData( false );
	OnStyleLBChanged();
	updateFont();
	updateLog();
	OnUpdateModify();
}

void RDOTracerLogOptions::OnUpdateModify()
{
	UpdateData();
	updateLog();
}

void RDOTracerLogOptions::updateFont()
{
	CComboBox* fontNameCB = (CComboBox*)GetDlgItem( IDC_FONTNAME_COMBO );
	if ( fontNameCB ) {
		int index = fontNameCB->FindStringExact( -1, sheet->logStyle.font.name );
		if ( index != CB_ERR ) {
			fontNameCB->SetCurSel( index );
		} else {
			fontNameCB->SetCurSel( -1 );
		}
	}

	CComboBox* fontSizeCB = (CComboBox*)GetDlgItem( IDC_FONTSIZE_COMBO );
	if ( fontSizeCB ) {
		CString s;
		s.Format( "%d", sheet->logStyle.font.size );
		int index = fontSizeCB->FindStringExact( -1, s );
		if ( index != CB_ERR ) {
			fontSizeCB->SetCurSel( index );
		} else {
			fontSizeCB->SetCurSel( -1 );
		}
	}
	
	RDOFontStyle fontStyle = sheet->logStyle.font.style;

	CButton* fontBoldB = (CButton*)GetDlgItem( IDC_FONTBOLD_CHECK );
	fontBoldB->SetCheck( fontStyle & RDOFS_BOLD );

	CButton* fontItalicB = (CButton*)GetDlgItem( IDC_FONTITALIC_CHECK );
	fontItalicB->SetCheck( fontStyle & RDOFS_ITALIC );

	CButton* fontUnderlineB = (CButton*)GetDlgItem( IDC_FONTUNDERLINE_CHECK );
	fontUnderlineB->SetCheck( fontStyle & RDOFS_UNDERLINE );
}

void RDOTracerLogOptions::updateButtons()
{
	CButton* button = (CButton*)GetDlgItem( IDC_LOG_RESET_BUTTON );
	if ( button )
		button->EnableWindow( sheet->logStyle != currentStyle );
	button = (CButton*)GetDlgItem( IDC_LOG_DEFAULT_BUTTON );
	if ( button )
		button->EnableWindow( sheet->logStyle != defaultStyle );
}

void RDOTracerLogOptions::updateLog()
{
	if ( log ) log.setStyle( sheet->logStyle );
	updateButtons();
	SetModified( sheet->logStyle != sheet->prevStyle );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOTracerOptions, CPropertySheet )

	ON_WM_HELPINFO( )

END_MESSAGE_MAP()

RDOTracerOptions::RDOTracerOptions( const RDOTracerLogStyle& style ):
	CPropertySheet(),
	logOptions( NULL )
{
	CString s;
	s.LoadString( IDS_OPTIONS );
	SetTitle( s );

	logStyle  = style;
	prevStyle = style;

	logOptions  = new RDOTracerLogOptions( *this );
	AddPage( logOptions );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOTracerOptions::~RDOTracerOptions()
{
	if ( logOptions ) delete logOptions;
}

void RDOTracerOptions::apply()
{
	((RDOTracerMainFrame*)AfxGetMainWnd())->getLog()->setStyle( logStyle );
	logStyle.save();
	prevStyle = logStyle;
}

int CALLBACK RDOTracerOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam)
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
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return;

	CPropertyPage* page = GetActivePage( );
	if ( page == editorOptions )
		filename += "::/html/work_options.htm#edit";
	if ( page == colorOptions )
		filename += "::/html/work_options.htm#color";
	HtmlHelp( ::GetDesktopWindow(), filename, HH_DISPLAY_TOPIC, NULL );
}

BOOL RDOTracerOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	CString filename = rdoEditorApp.getFullHelpFileName();
	if ( filename.IsEmpty() ) return TRUE;

	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
		return HtmlHelp( ::GetDesktopWindow(), filename, HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
*/