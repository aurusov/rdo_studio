#ifndef RDOTRACEROPTIONS_H
#define RDOTRACEROPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdocolorcombobox.h>
#include "tracer_ctrls/rdotracerlogctrl.h"
#include "tracer_ctrls/rdotracerlogstyle.h"
#include "trace_files/rdostudiochartdoc.h"
#include "trace_files/rdostudiochartview.h"
#include "trace_files/rdostudiochartviewstyle.h"
#include "trace_files/rdotracerserie.h"
#include "trace_files/rdotracervalues.h"
#include "../resource.h"

class RDOTracerOptions;

// ----------------------------------------------------------------------------
// ---------- RDOTracerOptionsColorsStyles
// ----------------------------------------------------------------------------
class RDOTracerOptionsColorsStyles: public CPropertyPage
{
private:

	class STYLEObject;

	class STYLEProperty {
	public:
		STYLEObject* object;
		std::string name;

		rdoStyle::RDOStyleFont::style& font_style;

		COLORREF& fg_color;
		COLORREF& bg_color;
		COLORREF& fg_disable_color;
		COLORREF& bg_disable_color;

		STYLEProperty( STYLEObject* _object, std::string _name, rdoStyle::RDOStyleFont::style& _font_style, COLORREF& _fg_color, COLORREF& _bg_color, COLORREF& _fg_disable_color = null_fg_color, COLORREF& _bg_disable_color = null_bg_color ): object( _object ), name( _name ), font_style( _font_style ), fg_color( _fg_color ), bg_color( _bg_color ), fg_disable_color( _fg_disable_color ), bg_disable_color( _bg_disable_color ) {};
	};

	class STYLEObject {
	public:
		enum Type { none = 0, all, trace, chart } type;
		std::string&                   font_name;
		int&                           font_size;
		bool                           font_fixed;

		std::list< std::string > themes;
		std::list< STYLEProperty* > properties;

		STYLEObject( const Type _type, std::string& _font_name, int& _font_size, const bool _font_fixed = true ): type( _type ), font_name( _font_name ), font_size( _font_size ), font_fixed( _font_fixed ) {};
		~STYLEObject() {
			std::list< STYLEProperty* >::iterator it = properties.begin();
			while ( it != properties.end() ) {
				delete *it++;
			};
		}
	};

	class STYLEFont {
	public:
		std::string name;
		bool fixed;

		STYLEFont(): name( "" ), fixed( true ) {};
		STYLEFont( std::string _name, bool _fixed = true ): name( _name ), fixed( _fixed ) {};
	};

	std::list< STYLEObject* > objects;
	std::list< STYLEFont > fonts;
	bool isCurrentFixed;
	static int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam );
	void loadFontsIntoCombo( bool fixed = true );

	RDOTracerOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	STYLEProperty*    getCurrentProperty() const;
	STYLEObject*      getCurrentObject() const;
	STYLEObject::Type getCurrentObjectType() const;

	STYLEObject::Type previewAs;
	void setPreviewAsCombo( STYLEObject::Type type );

	std::string all_font_name;
	int         all_font_size;
	COLORREF    all_fg_color;
	COLORREF    all_bg_color;
	bool        use_all_fg_color;
	bool        use_all_bg_color;
	void updatePropOfAllObject();

	rdoStyle::RDOStyleFont::style        null_font_style;
	static COLORREF                      null_fg_color;
	static COLORREF                      null_bg_color;

	void updateStyleItem();
	void updateTheme();

protected:
	//{{AFX_DATA(RDOTracerOptionsColorsStyles)
	enum { IDD = IDD_OPTIONS_STYLESANDCOLORS };
	CStatic	m_title_fontSizeStatic;
	CStatic	m_leg_fontSizeStatic;
	CComboBox	m_leg_fontSizeCombo;
	CComboBox	m_title_fontSizeCombo;
	CStatic	m_tickWidthStatic;
	CEdit	m_tickWidth;
	CStatic	m_vertBorderStatic;
	CEdit	m_vertBorder;
	CStatic	m_horzBorderStatic;
	CEdit	m_horzBorder;
	CComboBox	m_theme;
	CStatic	m_previewAsStatic;
	CStatic	m_bgColorStatic;
	CStatic	m_fgColorStatic;
	CButton	m_bgColorButton;
	CButton	m_fgColorButton;
	CButton	m_fontStyleUnderline;
	CButton	m_fontStyleItalic;
	CButton	m_fontStyleBold;
	CComboBox	m_fontSize;
	CComboBox	m_previewAs;
	CComboBox	m_fontName;
	CTreeCtrl	m_styleItem;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOTracerOptionsColorsStyles)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerOptionsColorsStyles)
	virtual BOOL OnInitDialog();
	afx_msg void OnStyleItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPreviewAsChanged();
	afx_msg void OnFontNameChanged();
	afx_msg void OnFontSizeChanged();
	afx_msg void OnFontStyleBoldChanged();
	afx_msg void OnFontStyleItalicChanged();
	afx_msg void OnFontStyleUnderlineChanged();
	afx_msg void OnFgColorChanged();
	afx_msg void OnBgColorChanged();
	afx_msg void OnFgColorClicked();
	afx_msg void OnBgColorClicked();
	afx_msg void OnThemeChanged();
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOTracerOptionsColorsStyles( RDOTracerOptions& _sheet );
	virtual ~RDOTracerOptionsColorsStyles();
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerOptions
// ----------------------------------------------------------------------------
class RDOTracerOptions: public CPropertySheet
{
friend class RDOTracerOptionsColorsStyles;

private:
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	RDOStudioChartViewStyle          style_chart;

	RDOTracerOptionsColorsStyles* styles;

	rdoTracerLog::RDOTracerLogCtrl preview_trace;

	bool                           chart_need_delete;
	RDOStudioChartDoc*             preview_chart_doc;
	RDOStudioChartView*            preview_chart;
	std::vector<RDOTracerTimeNow>  preview_times;
	RDOTracerSerie                 preview_serie;

	void updateStyles();
	void apply() const;
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

protected:
	//{{AFX_VIRTUAL(RDOTracerOptions)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerOptions)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOTracerOptions();
	virtual ~RDOTracerOptions();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOTRACEROPTIONS_H
