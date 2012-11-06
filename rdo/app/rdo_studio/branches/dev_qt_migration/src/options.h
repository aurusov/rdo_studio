/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/options.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_OPTIONS_H_
#define _RDO_STUDIO_MFC_OPTIONS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdocolorcombobox.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view_style.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/src/chart/view_style.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/src/frame/style.h"
#include "app/rdo_studio_mfc/src/frame/options_ctrl.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsGeneral
// --------------------------------------------------------------------------------
class RDOStudioOptions;

class RDOStudioOptionsGeneral: public CPropertyPage
{
friend class RDOStudioOptions;

private:
	RDOStudioOptions* sheet;

protected:
	enum { IDD = IDD_OPTIONS_GENERAL };
	BOOL	m_setup;
	BOOL	m_checkInFuture;
	BOOL	m_openLastProject;
	BOOL	m_showFullName;

private:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsGeneral( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsGeneral();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsEditor
// --------------------------------------------------------------------------------
class RDOStudioOptionsEditor: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

protected:
	enum { IDD = IDD_OPTIONS_EDITOR };
	BOOL	m_bufferClearAuto;
	int		m_bufferDelay;
	BOOL	m_codecompUse;
	int		m_codecompShowFullList;
	BOOL	m_marginFold;
	BOOL	m_marginBookmark;
	BOOL	m_marginLineNumber;

private:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUseAutoCompleteCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateModify();
	afx_msg void OnClearAutoCheck();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsEditor( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsEditor();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsTabs
// --------------------------------------------------------------------------------
class RDOStudioOptionsTabs: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

protected:
	enum { IDD = IDD_OPTIONS_TABS };
	BOOL	m_tabUse;
	int		m_tabSize;
	BOOL	m_tabUseTabIndent;
	int		m_tabIndentSize;
	int		m_tabBackspaceUntabs;
	BOOL	m_tabAutoIndent;

private:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsTabs( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsTabs();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsColorsStyles
// --------------------------------------------------------------------------------
class RDOStudioOptionsColorsStyles: public CPropertyPage
{
private:

	class STYLEObject;

	class STYLEProperty {
	public:
		STYLEObject* object;
		tstring name;

		rdoStyle::RDOStyleFont::style& font_style;

		COLORREF& fg_color;
		COLORREF& bg_color;
		COLORREF& fg_disable_color;
		COLORREF& bg_disable_color;

		STYLEProperty( STYLEObject* _object, tstring _name, rdoStyle::RDOStyleFont::style& _font_style, COLORREF& _fg_color, COLORREF& _bg_color, COLORREF& _fg_disable_color = null_fg_color, COLORREF& _bg_disable_color = null_bg_color ): object( _object ), name( _name ), font_style( _font_style ), fg_color( _fg_color ), bg_color( _bg_color ), fg_disable_color( _fg_disable_color ), bg_disable_color( _bg_disable_color ) {};
	};

	class STYLEObject {
	public:
		enum Type { none = 0, all, source, build, debug, trace, results, find, chart, frame } type;
		tstring&                       font_name;
		int&                           font_size;
		rbool                          font_fixed;
		rbool&                         wordwrap;
		rbool&                         horzscrollbar;
		rbool&                         warning;
		rdoEditCtrl::RDOBookmarkStyle& bookmarkstyle;
		rdoEditor::RDOFoldStyle&       foldstyle;
		rbool&                         commentfold;

		std::list< tstring > themes;
		std::list< STYLEProperty* > properties;

		STYLEObject( const Type _type, tstring& _font_name, int& _font_size, const rbool _font_fixed = true, rbool& _wordwrap = null_wordwrap, rbool& _horzscrollbar = null_horzscrollbar, rdoEditCtrl::RDOBookmarkStyle& _bookmarkstyle = null_bookmarkstyle, rdoEditor::RDOFoldStyle& _foldstyle = null_foldstyle, rbool& _commentfold = null_commentfold, rbool& _warning = null_warning ):
			type( _type ),
			font_name( _font_name ),
			font_size( _font_size ),
			font_fixed( _font_fixed ),
			wordwrap( _wordwrap ),
			horzscrollbar( _horzscrollbar ),
			warning( _warning ),
			bookmarkstyle( _bookmarkstyle ),
			foldstyle( _foldstyle ),
			commentfold( _commentfold )
		{
		};
		~STYLEObject() {
			std::list< STYLEProperty* >::iterator it = properties.begin();
			while ( it != properties.end() ) {
				delete *it++;
			};
		}
	};

	class STYLEFont {
	public:
		tstring name;
		rbool   fixed;

		STYLEFont(): name( "" ), fixed( true ) {};
		STYLEFont( tstring _name, rbool _fixed = true ): name( _name ), fixed( _fixed ) {};
	};

	std::list< STYLEObject* > objects;
	std::list< STYLEFont > fonts;
	rbool isCurrentFixed;
	static int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam );
	void loadFontsIntoCombo( rbool fixed = true );

	RDOStudioOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	STYLEProperty*    getCurrentProperty() const;
	STYLEObject*      getCurrentObject() const;
	STYLEObject::Type getCurrentObjectType() const;

	STYLEObject::Type previewAs;
	void setPreviewAsCombo( STYLEObject::Type type );

	tstring     all_font_name;
	int         all_font_size;
	COLORREF    all_fg_color;
	COLORREF    all_bg_color;
	rbool       use_all_fg_color;
	rbool       use_all_bg_color;
	void updatePropOfAllObject();

	rdoStyle::RDOStyleFont::style        null_font_style;
	static COLORREF                      null_fg_color;
	static COLORREF                      null_bg_color;
	static rbool                         null_wordwrap;
	static rbool                         null_horzscrollbar;
	static rbool                         null_warning;
	static rbool                         null_commentfold;
	static rdoEditCtrl::RDOBookmarkStyle null_bookmarkstyle;
	static rdoEditor::RDOFoldStyle       null_foldstyle;

	void updateStyleItem();
	void updateTheme();

protected:
	enum { IDD = IDD_OPTIONS_STYLESANDCOLORS };
	CButton	m_warning;
	CButton	m_commentGroupButton;
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
	CButton	m_horzScrollBar;
	CButton	m_wordWrap;
	CComboBox	m_fold;
	CStatic	m_foldStatic;
	CStatic	m_bookmarkStatic;
	CComboBox	m_bookmark;
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

private:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

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
	afx_msg void OnWordWrapClicked();
	afx_msg void OnHorzScrollBarClicked();
	afx_msg void OnBookmarkChanged();
	afx_msg void OnFoldChanged();
	afx_msg void OnThemeChanged();
	afx_msg void OnUpdateModify();
	afx_msg void OnCommentGroupCheck();
	afx_msg void OnWarningCheck();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsColorsStyles( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsColorsStyles();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsPlugins
// --------------------------------------------------------------------------------
class RDOStudioPlugin;

class RDOStudioOptionsPlugins: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

	rbool sortPluginNameAsceding;
	rbool sortPluginVersionAsceding;
	rbool sortPluginRunModeAsceding;
	rbool sortPluginStateAsceding;
	rbool sortPluginDescriptionAsceding;

	UINT timer;

	void updateRunModeInGrid( const RDOStudioPlugin* plugin, const int index );
	void updateStateInGrid( const RDOStudioPlugin* plugin, const int index );
	void updateControls( const RDOStudioPlugin* plugin );

protected:
	enum { IDD = IDD_OPTIONS_PLUGINS };
	CButton	m_restoreStateCheckBox;
	CButton	m_stopButton;
	CButton	m_startButton;
	CComboBox	m_runModeComboBox;
	CButton	m_runModeButton;
	CStatic	m_runModeStatic;
	CListCtrl	m_pluginList;

private:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	virtual BOOL OnInitDialog();
	afx_msg void OnPluginListColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluginListSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluginRunModeComboBoxChanged();
	afx_msg void OnPluginRunModeButtonClicked();
	afx_msg void OnPluginStart();
	afx_msg void OnPluginStop();
	afx_msg void OnPluginRestoreStateCheckBoxClicked();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsPlugins( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsPlugins();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptions
// --------------------------------------------------------------------------------
class RDOStudioOptions: public CPropertySheet
{
friend class RDOStudioOptionsGeneral;
friend class RDOStudioOptionsEditor;
friend class RDOStudioOptionsTabs;
friend class RDOStudioOptionsColorsStyles;
friend class RDOStudioOptionsPlugins;

private:
	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	RDOStudioChartViewStyle          style_chart;
	RDOStudioFrameStyle              style_frame;

	RDOStudioOptionsGeneral*      general;
	RDOStudioOptionsEditor*       editor;
	RDOStudioOptionsTabs*         tabs;
	RDOStudioOptionsColorsStyles* styles;
	RDOStudioOptionsPlugins*      plugins;

	rdoEditor::RDOEditorEdit       preview_editor;
	rdoEditCtrl::RDOBuildEdit      preview_build;
	rdoEditCtrl::RDODebugEdit      preview_debug;
	rdoTracerLog::RDOTracerLogCtrl preview_trace;
	rdoEditor::RDOEditorResults    preview_results;
	rdoEditCtrl::RDOFindEdit       preview_find;

	rbool                          chart_need_delete;
	RDOStudioChartDoc*             preview_chart_doc;
	RDOStudioChartView*            preview_chart;
	std::vector<RDOTracerTimeNow>  preview_times;
	RDOTracerSerie                 preview_serie;

	RDOStudioFrameOptionsCtrl      preview_frame;

	typedef std::map<ruint,tstring> mapKeyAndUrl;
	mapKeyAndUrl                   m_keyAndUrl;

	tstring      resolveKeyAndUrl (ruint helpInfo);
	void         buildMap         ();
	void         onHelpButton();

	void updateStyles();
	void apply() const;
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);

	virtual BOOL ContinueModal();

	afx_msg BOOL OnHelpInfo(PTR(HELPINFO) pHelpInfo);
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptions();
	virtual ~RDOStudioOptions();
};

#endif // _RDO_STUDIO_MFC_OPTIONS_H_
