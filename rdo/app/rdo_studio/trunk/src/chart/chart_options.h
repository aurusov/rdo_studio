/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_options.h
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_OPTIONS_H_
#define _RDO_STUDIO_CHART_OPTIONS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdocolorcombobox.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsChart
// --------------------------------------------------------------------------------
class RDOStudioChartOptions;

class RDOStudioChartOptionsChart: public CPropertyPage
{
public:
	RDOStudioChartOptionsChart(RDOStudioChartOptions& _sheet);
	virtual ~RDOStudioChartOptionsChart();

	void apply() const;

protected:
	enum { IDD = IDD_CHART_OPTIONS_CHART };
	CComboBox m_AxisSerieCombo;
	CButton   m_DrawLegend;
	int       m_ValCountX;
	int       m_ValCountY;
	CString   m_ChartTitle;
	int       m_AxisSerie;

private:
	RDOStudioChartOptions* m_pSheet;

	rbool checkValues() const;

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsSeries
// --------------------------------------------------------------------------------
class RDOStudioChartOptionsSeries: public CPropertyPage
{
public:
	RDOStudioChartOptionsSeries(RDOStudioChartOptions& _sheet);
	virtual ~RDOStudioChartOptionsSeries();

	void apply() const;

protected:
	rbool getModified() const;
	void  restoreValues();

	enum { IDD = IDD_CHART_OPTIONS_SERIES };
	CButton    m_TranspMarker;
	CComboBox  m_SerieCombo;
	CButton    m_DrawInLegend;
	CEdit      m_MarkerSize;
	CButton    m_DrawMarker;
	CComboBox  m_Marker;
	CEdit      m_SerieTitle;
	int        m_sizeMarker;

private:
	RDOStudioChartOptions* m_pSheet;
	ChartSerie*            m_pSerie;
	RDOColorComboBox       m_colorCB;

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
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptions
// --------------------------------------------------------------------------------
class ChartView;

class RDOStudioChartOptions: public CPropertySheet
{
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;
public:
	RDOStudioChartOptions(ChartView* pView);
	virtual ~RDOStudioChartOptions();

private:
	RDOStudioChartOptionsChart*  m_pChart;
	RDOStudioChartOptionsSeries* m_pSeries;
	ChartView*                   m_pView;

	void apply() const;
	void onHelpButton();

	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_CHART_OPTIONS_H_
