#ifndef RDOSTUDIOCHARTOPTIONS_H
#define RDOSTUDIOCHARTOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "ui/mfc_ctrls/rdocolorcombobox.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/resource.h"

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartOptionsChart
// --------------------------------------------------------------------------------
class RDOStudioChartOptions;

class RDOStudioChartOptionsChart: public CPropertyPage
{
private:
	RDOStudioChartOptions* sheet;

protected:
	//{{AFX_DATA(RDOStudioChartOptionsChart)
	enum { IDD = IDD_CHART_OPTIONS_CHART };
	CComboBox	m_AxisSerieCombo;
	CButton	m_DrawLegend;
	int		m_ValCountX;
	int		m_ValCountY;
	CString	m_ChartTitle;
	int		m_AxisSerie;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioChartOptionsChart)
	public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioChartOptionsChart)
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool checkValues() const;

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
	RDOStudioDocSerie* serie;

	RDOColorComboBox ColorCB;

protected:
	
	bool getModified() const;
	void restoreValues();

	//{{AFX_DATA(RDOStudioChartOptionsSeries)
	enum { IDD = IDD_CHART_OPTIONS_SERIES };
	CButton	m_TranspMarker;
	CComboBox	m_SerieCombo;
	CButton	m_DrawInLegend;
	CEdit	m_MarkerSize;
	CButton	m_DrawMarker;
	CComboBox	m_Marker;
	CEdit	m_SerieTitle;
	int		m_sizeMarker;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioChartOptionsSeries)
	public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioChartOptionsSeries)
	afx_msg void OnUpdateModify();
	afx_msg void OnColorButton();
	afx_msg void OnSelchangeSeriesCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool checkValues() const;

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
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

protected:
	//{{AFX_VIRTUAL(RDOStudioChartOptions)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioChartOptions)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartOptions( RDOStudioChartView* _view );
	virtual ~RDOStudioChartOptions();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOCHARTOPTIONS_H
