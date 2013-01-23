/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/options.h
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHART_OPTIONS_H_
#define _RDO_STUDIO_MFC_CHART_OPTIONS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdocolorcombobox.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsChart
// --------------------------------------------------------------------------------
class RDOStudioChartOptions;

class RDOStudioChartOptionsChart: public CPropertyPage
{
private:
	RDOStudioChartOptions* sheet;

protected:
	enum { IDD = IDD_CHART_OPTIONS_CHART };
	CComboBox	m_AxisSerieCombo;
	CButton	m_DrawLegend;
	int		m_ValCountX;
	int		m_ValCountY;
	CString	m_ChartTitle;
	int		m_AxisSerie;

private:
	rbool checkValues() const;

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartOptionsChart( RDOStudioChartOptions& _sheet );
	virtual ~RDOStudioChartOptionsChart();

	void apply() const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsSeries
// --------------------------------------------------------------------------------
class RDOStudioChartOptionsSeries: public CPropertyPage
{
private:
	RDOStudioChartOptions* sheet;
	ChartSerie* serie;

	RDOColorComboBox ColorCB;

protected:
	
	rbool getModified() const;
	void  restoreValues();

	enum { IDD = IDD_CHART_OPTIONS_SERIES };
	CButton	m_TranspMarker;
	CComboBox	m_SerieCombo;
	CButton	m_DrawInLegend;
	CEdit	m_MarkerSize;
	CButton	m_DrawMarker;
	CComboBox	m_Marker;
	CEdit	m_SerieTitle;
	int		m_sizeMarker;

private:
	rbool checkValues() const;

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	afx_msg void OnColorButton();
	afx_msg void OnSelchangeSeriesCombo();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartOptionsSeries( RDOStudioChartOptions& _sheet );
	virtual ~RDOStudioChartOptionsSeries();

	void apply() const;
};
// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptions
// --------------------------------------------------------------------------------
class RDOStudioChartView;

class RDOStudioChartOptions: public CPropertySheet
{
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;

private:
	RDOStudioChartOptionsChart*  chart;
	RDOStudioChartOptionsSeries* series;
	
	RDOStudioChartView*          view;

	void apply() const;
	void onHelpButton();

private:
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartOptions( RDOStudioChartView* _view );
	virtual ~RDOStudioChartOptions();
};

#endif // _RDO_STUDIO_MFC_CHART_OPTIONS_H_
