/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/options.cpp
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/options.h"
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_X_Y_M   2
#define MAX_X_Y     100
#define MAX_M       6

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsChart
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptionsChart, CPropertyPage)
	ON_EN_CHANGE(IDC_CHART_TITLE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_DRAW_LEGEND_CHECK, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_AXIS_SERIES, OnUpdateModify)
	ON_EN_CHANGE(IDC_IDC_VAL_COUNT_Y_EDIT, OnUpdateModify)
	ON_EN_CHANGE(IDC_IDC_VAL_COUNT_X_EDIT, OnUpdateModify)
END_MESSAGE_MAP()

RDOStudioChartOptionsChart::RDOStudioChartOptionsChart( RDOStudioChartOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{
	m_ValCountX = sheet->view->valueCountX;
	m_ValCountY = sheet->view->valueCountY;
	m_ChartTitle = sheet->view->GetDocument()->getTitle().c_str();
	m_AxisSerie = -1;

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioChartOptionsChart::~RDOStudioChartOptionsChart()
{
}

void RDOStudioChartOptionsChart::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_AXIS_SERIES, m_AxisSerieCombo);
	DDX_Control(pDX, IDC_DRAW_LEGEND_CHECK, m_DrawLegend);
	DDX_Text(pDX, IDC_IDC_VAL_COUNT_X_EDIT, m_ValCountX);
	DDX_Text(pDX, IDC_IDC_VAL_COUNT_Y_EDIT, m_ValCountY);
	DDX_Text(pDX, IDC_CHART_TITLE_EDIT, m_ChartTitle);
	DDX_CBIndex(pDX, IDC_AXIS_SERIES, m_AxisSerie);
}

BOOL RDOStudioChartOptionsChart::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	RDOStudioChartDoc* doc = sheet->view->GetDocument();

	for ( std::vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
		m_AxisSerieCombo.AddString( (*it)->serie->getTitle().c_str() );
	}
	
	m_AxisSerieCombo.SetCurSel( sheet->view->GetDocument()->getSerieIndex( sheet->view->yAxis ) );

	m_DrawLegend.SetCheck( sheet->view->needDrawLegend );

	return TRUE;
}

void RDOStudioChartOptionsChart::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

rbool RDOStudioChartOptionsChart::checkValues() const
{
	int valCount_Y = GetDlgItemInt(IDC_IDC_VAL_COUNT_Y_EDIT);
	CEdit* edit_Y = (CEdit*) GetDlgItem(IDC_IDC_VAL_COUNT_Y_EDIT);
	int valCount_X = GetDlgItemInt(IDC_IDC_VAL_COUNT_X_EDIT);
	CEdit* edit_X = (CEdit*) GetDlgItem(IDC_IDC_VAL_COUNT_X_EDIT);

	int val_corrected;
	CString ctrl_caption;

	if (valCount_X < MIN_X_Y_M || valCount_X > MAX_X_Y) {
		((CStatic*) GetDlgItem(IDC_VAL_COUNT_X_STATIC))->GetWindowText(ctrl_caption);
		AfxGetMainWnd()->MessageBox(rdo::format(IDS_CHART_OPTIONS_INT_VERIFY, ctrl_caption, MIN_X_Y_M, MAX_X_Y ).c_str(), NULL, MB_OK | MB_ICONWARNING);

		if (valCount_X < MIN_X_Y_M)
		{
			val_corrected = MIN_X_Y_M;
		}
		else if (valCount_X > MAX_X_Y)
		{
			val_corrected = MAX_X_Y;
		}
		else
		{
			val_corrected = ~0;
			NEVER_REACH_HERE;
		}
		edit_X->SetSel(0, -1);
		edit_X->ReplaceSel(rdo::format( "%d", val_corrected ).c_str());
		edit_X->SetFocus();
		return false;
	}

	if (valCount_Y < MIN_X_Y_M || valCount_Y > MAX_X_Y) {
		((CStatic*) GetDlgItem(IDC_VAL_COUNT_Y_STATIC))->GetWindowText(ctrl_caption);
		AfxGetMainWnd()->MessageBox(rdo::format(IDS_CHART_OPTIONS_INT_VERIFY, ctrl_caption, MIN_X_Y_M, MAX_X_Y ).c_str(), NULL, MB_OK | MB_ICONWARNING);
		
		if (valCount_Y < MIN_X_Y_M)
		{
			val_corrected = MIN_X_Y_M;
		}
		else if (valCount_Y > MAX_X_Y)
		{
			val_corrected = MAX_X_Y;
		}
		else
		{
			val_corrected = 0;
			NEVER_REACH_HERE;
		}
		edit_Y->SetSel(0, -1);
		edit_Y->ReplaceSel(rdo::format( "%d", val_corrected ).c_str());
		edit_Y->SetFocus();
		return false;
	}

	return true;
}

BOOL RDOStudioChartOptionsChart::OnKillActive() 
{
	if (!checkValues()) return 0;
	
	return CPropertyPage::OnKillActive();
}

BOOL RDOStudioChartOptionsChart::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioChartOptionsChart::OnUpdateModify() 
{
	UpdateData();
	
	RDOStudioChartView* view = sheet->view;
	rbool legend = m_DrawLegend.GetCheck() ? true : false;

	SetModified( legend != view->needDrawLegend || m_ValCountX != view->valueCountX || m_ValCountY != view->valueCountY || m_ChartTitle != view->GetDocument()->getTitle().c_str() || m_AxisSerie != view->GetDocument()->getSerieIndex( view->yAxis ) );
}

void RDOStudioChartOptionsChart::apply() const
{
	if ( m_AxisSerie != -1 )
		sheet->view->yAxis = sheet->view->GetDocument()->series.at( m_AxisSerie );
	else
		sheet->view->yAxis = NULL;
	rbool legend = m_DrawLegend.GetCheck() ? true : false;
	sheet->view->needDrawLegend = legend;
	sheet->view->valueCountX = m_ValCountX;
	sheet->view->valueCountY = m_ValCountY;
	sheet->view->GetDocument()->SetTitle( m_ChartTitle );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsSeries
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptionsSeries, CPropertyPage)
	ON_EN_CHANGE(IDC_SERIE_TITLE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	ON_CBN_SELCHANGE(IDC_SERIES_COMBO, OnSelchangeSeriesCombo)
	ON_CBN_SELCHANGE(IDC_COLOR_COMBO, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_MARKER_COMBO, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARKER_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_MARKER_SIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_LEGEND_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_TRANSP_MARKER_CHECK, OnUpdateModify)
END_MESSAGE_MAP()

RDOStudioChartOptionsSeries::RDOStudioChartOptionsSeries( RDOStudioChartOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	serie( NULL )
{
	m_sizeMarker = 2;

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioChartOptionsSeries::~RDOStudioChartOptionsSeries()
{
}

rbool RDOStudioChartOptionsSeries::getModified() const
{
	rbool res = false;
	if ( serie ) {
		CString title;
		m_SerieTitle.GetWindowText( title );
		rbool marker = m_DrawMarker.GetCheck() ? true : false;
		rbool legend = m_DrawInLegend.GetCheck() ? true : false;
		rbool transp = m_TranspMarker.GetCheck() ? true : false;
		res = title != serie->docSerieTitle.c_str() || ColorCB.getCurrentColor() != serie->color || m_Marker.GetCurSel() != serie->marker || marker != serie->needDrawMarker || m_sizeMarker != serie->marker_size || legend != serie->showInLegend || transp != serie->transparentMarker;
	}
	return res;
}

void RDOStudioChartOptionsSeries::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TRANSP_MARKER_CHECK, m_TranspMarker);
	DDX_Control(pDX, IDC_SERIES_COMBO, m_SerieCombo);
	DDX_Control(pDX, IDC_LEGEND_CHECK, m_DrawInLegend);
	DDX_Control(pDX, IDC_MARKER_SIZE_EDIT, m_MarkerSize);
	DDX_Control(pDX, IDC_MARKER_CHECK, m_DrawMarker);
	DDX_Control(pDX, IDC_MARKER_COMBO, m_Marker);
	DDX_Control(pDX, IDC_SERIE_TITLE_EDIT, m_SerieTitle);
	DDX_Text(pDX, IDC_MARKER_SIZE_EDIT, m_sizeMarker);

	DDX_Control( pDX, IDC_COLOR_COMBO, ColorCB );
}

BOOL RDOStudioChartOptionsSeries::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = static_cast<CComboBox*>(GetDlgItem( IDC_SERIES_COMBO ))->GetItemHeight( -1 );
	ColorCB.setItemHeight( itemHeight );
	ColorCB.insertBaseColors();

	RDOStudioChartDoc* doc = sheet->view->GetDocument();

	for ( std::vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
		m_SerieCombo.AddString( (*it)->serie->getTitle().c_str() );
	}
	
	m_SerieCombo.SetCurSel( 0 );
	OnSelchangeSeriesCombo();

	return TRUE;
}

void RDOStudioChartOptionsSeries::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioChartOptionsSeries::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

rbool RDOStudioChartOptionsSeries::checkValues() const
{
	int marker_size = GetDlgItemInt(IDC_MARKER_SIZE_EDIT);
	CEdit* edit_marker_size = (CEdit*) GetDlgItem(IDC_MARKER_SIZE_EDIT);

	int val_corrected;
	CString ctrl_caption;

	if (marker_size < MIN_X_Y_M || marker_size > MAX_M) {
		((CStatic*) GetDlgItem(IDC_MAKER_SIZE_STATIC))->GetWindowText(ctrl_caption);
		AfxGetMainWnd()->MessageBox(rdo::format(IDS_CHART_OPTIONS_INT_VERIFY, ctrl_caption, MIN_X_Y_M, MAX_M ).c_str(), NULL, MB_OK | MB_ICONWARNING);

		if (marker_size < MIN_X_Y_M)
		{
			val_corrected = MIN_X_Y_M;
		}
		else if (marker_size > MAX_M)
		{
			val_corrected = MAX_M;
		}
		else
		{
			val_corrected = ~0;
			NEVER_REACH_HERE;
		}
		edit_marker_size->SetSel(0, -1);
		edit_marker_size->ReplaceSel(rdo::format( "%d", val_corrected ).c_str());
		edit_marker_size->SetFocus();
		return false;
	}

	return true;
}

BOOL RDOStudioChartOptionsSeries::OnKillActive() 
{
	if (!checkValues()) return 0;
	
	return CPropertyPage::OnKillActive();
}

void RDOStudioChartOptionsSeries::OnUpdateModify() 
{
	if ( serie ) {
		UpdateData();
		SetModified( getModified() );
	}
}

void RDOStudioChartOptionsSeries::apply() const
{
	if ( serie ) {
		CString title;
		m_SerieTitle.GetWindowText( title );
		serie->docSerieTitle = title;
		serie->color = ColorCB.getCurrentColor();
		serie->marker = static_cast<RDOTracerSerieMarker>(m_Marker.GetCurSel());
		rbool marker = m_DrawMarker.GetCheck() ? true : false;
		serie->needDrawMarker = marker;
		serie->marker_size = m_sizeMarker;
		rbool legend = m_DrawInLegend.GetCheck() ? true : false;
		serie->showInLegend = legend;
		rbool transp = m_TranspMarker.GetCheck() ? true : false;
		serie->transparentMarker = transp;
	}
}

void RDOStudioChartOptionsSeries::restoreValues()
{
	if ( serie ) {
		m_SerieTitle.SetWindowText( serie->docSerieTitle.c_str() );
		ColorCB.insertColor( serie->color );
		ColorCB.setCurrentColor( serie->color );
		m_Marker.SetCurSel( serie->marker );
		m_DrawMarker.SetCheck( serie->needDrawMarker );
		m_MarkerSize.SetWindowText( rdo::format( "%d", serie->marker_size).c_str() );
		m_DrawInLegend.SetCheck( serie->showInLegend );
		m_TranspMarker.SetCheck( serie->transparentMarker );
	}
}

void RDOStudioChartOptionsSeries::OnSelchangeSeriesCombo() 
{
	if (!checkValues()) {
		m_SerieCombo.SetCurSel( sheet->view->GetDocument()->getSerieIndex( serie ) );
		return;
	}
	int ser = m_SerieCombo.GetCurSel();
	rbool enable = ser != -1;
	m_SerieTitle.EnableWindow( enable );
	ColorCB.EnableWindow( enable );
	m_Marker.EnableWindow( enable );
	m_DrawMarker.EnableWindow( enable );
	m_MarkerSize.EnableWindow( enable );
	m_DrawInLegend.EnableWindow( enable );
	m_TranspMarker.EnableWindow( enable );
	if ( enable ) {
		if ( serie && getModified() ) {
			int res = AfxGetMainWnd()->MessageBox( rdo::format( IDS_CHART_OPTIONS_APPLY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
			rbool continue_flag = true;  
			switch ( res ) {
				case IDYES   : sheet->apply(); break;
				case IDNO    : restoreValues(); break;
				case IDCANCEL: {
					continue_flag = false;
					m_SerieCombo.SetCurSel( sheet->view->GetDocument()->getSerieIndex( serie ) );
					break;
				}
			}
			if ( !continue_flag ) return;
		}
		serie = sheet->view->GetDocument()->series.at( ser );
		restoreValues();
	} else {
		serie = NULL;
	}
}

void RDOStudioChartOptionsSeries::OnColorButton() 
{
	CColorDialog dlg( ColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		ColorCB.insertColor( dlg.GetColor() );
		OnUpdateModify();
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptions
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptions, CPropertySheet)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

RDOStudioChartOptions::RDOStudioChartOptions( RDOStudioChartView* _view ):
	CPropertySheet(),
	chart( NULL ),
	view( _view )
{
	SetTitle( rdo::format( IDS_CHART_OPTIONS ).c_str() );

	chart  = new RDOStudioChartOptionsChart( *this );
	series = new RDOStudioChartOptionsSeries( *this );

	AddPage( chart );
	AddPage( series );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioChartOptions::~RDOStudioChartOptions()
{
	if ( chart )  { delete chart; chart = NULL; }
	if ( series ) { delete series; series = NULL; }
}

void RDOStudioChartOptions::apply() const
{
	chart->apply();
	series->apply();
	view->GetDocument()->UpdateAllViews( NULL );
}

int CALLBACK RDOStudioChartOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM /*lParam*/)
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

void RDOStudioChartOptions::onHelpButton()
{
	/*QProcess* assistant = studioApp.chkQtAssistantWindow();
	if ( assistant->state() != assistant->Running ) return;
	QByteArray ba;

	CPropertyPage* page = GetActivePage( );
	if ( page == editor ) {
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options.htm#editor\n");
	} else if ( page == tabs ) {
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options.htm#tabs\n");
	} else if ( page == styles ) {
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options.htm#styles\n");
	}
	assistant->write(ba);*/
}

BOOL RDOStudioChartOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return TRUE;

	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
		return ::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
