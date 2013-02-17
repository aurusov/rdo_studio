/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/options.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_OPTIONS_H_
#define _RDO_STUDIO_OPTIONS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdocolorcombobox.h"
#include "app/rdo_studio/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio/src/tracer/ctrls/log_main_wnd.h"
#include "app/rdo_studio/src/tracer/ctrls/log_view_style.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/src/chart/chart_view_style.h"
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/src/frame/frame_style.h"
#include "app/rdo_studio/src/frame/frame_options_ctrl.h"
#include "app/rdo_studio/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsGeneral
// --------------------------------------------------------------------------------
class RDOStudioOptions;

class RDOStudioOptionsGeneral: public CPropertyPage
{
friend class RDOStudioOptions;

public:
	RDOStudioOptionsGeneral(REF(RDOStudioOptions) sheet);
	virtual ~RDOStudioOptionsGeneral();

protected:
	enum { IDD = IDD_OPTIONS_GENERAL };

	BOOL m_setup;
	BOOL m_checkInFuture;
	BOOL m_openLastProject;
	BOOL m_showFullName;

private:
	PTR(RDOStudioOptions) sheet;

	virtual void OnOK          ();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify      (WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsEditor
// --------------------------------------------------------------------------------
class RDOStudioOptionsEditor: public CPropertyPage
{
public:
	RDOStudioOptionsEditor(REF(RDOStudioOptions) sheet);
	virtual ~RDOStudioOptionsEditor();

protected:
	enum { IDD = IDD_OPTIONS_EDITOR };

	BOOL m_codecompUse;
	int  m_codecompShowFullList;
	BOOL m_marginFold;
	BOOL m_marginBookmark;
	BOOL m_marginLineNumber;

private:
	RDOStudioOptions* sheet;

	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUseAutoCompleteCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateModify();
	afx_msg void OnClearAutoCheck();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsTabs
// --------------------------------------------------------------------------------
class RDOStudioOptionsTabs: public CPropertyPage
{
public:
	RDOStudioOptionsTabs(REF(RDOStudioOptions) sheet);
	virtual ~RDOStudioOptionsTabs();

protected:
	enum { IDD = IDD_OPTIONS_TABS };

	BOOL m_tabUse;
	int  m_tabSize;
	BOOL m_tabUseTabIndent;
	int  m_tabIndentSize;
	int  m_tabBackspaceUntabs;
	BOOL m_tabAutoIndent;

private:
	PTR(RDOStudioOptions) sheet;

	virtual void OnOK          ();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify      (WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnUpdateModify();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsColorsStyles
// --------------------------------------------------------------------------------
class RDOStudioOptionsColorsStyles: public CPropertyPage
{
public:
	RDOStudioOptionsColorsStyles(RDOStudioOptions& _sheet);
	virtual ~RDOStudioOptionsColorsStyles();

protected:
	enum { IDD = IDD_OPTIONS_STYLESANDCOLORS };

	CButton   m_warning;
	CButton   m_commentGroupButton;
	CStatic   m_title_fontSizeStatic;
	CStatic   m_leg_fontSizeStatic;
	CComboBox m_leg_fontSizeCombo;
	CComboBox m_title_fontSizeCombo;
	CStatic   m_tickWidthStatic;
	CEdit     m_tickWidth;
	CStatic   m_vertBorderStatic;
	CEdit     m_vertBorder;
	CStatic   m_horzBorderStatic;
	CEdit     m_horzBorder;
	CComboBox m_theme;
	CButton   m_horzScrollBar;
	CButton   m_wordWrap;
	CComboBox m_fold;
	CStatic   m_foldStatic;
	CStatic   m_bookmarkStatic;
	CComboBox m_bookmark;
	CStatic   m_previewAsStatic;
	CStatic   m_bgColorStatic;
	CStatic   m_fgColorStatic;
	CButton   m_bgColorButton;
	CButton   m_fgColorButton;
	CButton   m_fontStyleUnderline;
	CButton   m_fontStyleItalic;
	CButton   m_fontStyleBold;
	CComboBox m_fontSize;
	CComboBox m_previewAs;
	CComboBox m_fontName;
	CTreeCtrl m_styleItem;

private:

	class STYLEObject;

	class STYLEProperty
	{
	public:
		STYLEObject* object;
		tstring name;

		rdoStyle::RDOStyleFont::style& font_style;

		COLORREF& fg_color;
		COLORREF& bg_color;
		COLORREF& fg_disable_color;
		COLORREF& bg_disable_color;

		STYLEProperty(STYLEObject* object, CREF(tstring) name, rdoStyle::RDOStyleFont::style& font_style, COLORREF& fg_color, COLORREF& bg_color, COLORREF& fg_disable_color = null_fg_color, COLORREF& bg_disable_color = null_bg_color)
			: object          (object    )
			, name            (name      )
			, font_style      (font_style)
			, fg_color        (fg_color  )
			, bg_color        (bg_color  )
			, fg_disable_color(fg_disable_color)
			, bg_disable_color(bg_disable_color)
		{};
	};
	typedef  std::list<PTR(STYLEProperty)>  PropertyList;

	class STYLEObject
	{
	public:
		enum Type
		{
			none = 0,
			all,
			source,
			build,
			debug,
			trace,
			results,
			find,
			chart,
			frame
		} type;

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
		PropertyList         properties;

		STYLEObject(Type type, tstring& font_name, int& font_size, rbool font_fixed = true, rbool& wordwrap = null_wordwrap, rbool& horzscrollbar = null_horzscrollbar, rdoEditCtrl::RDOBookmarkStyle& bookmarkstyle = null_bookmarkstyle, rdoEditor::RDOFoldStyle& foldstyle = null_foldstyle, rbool& commentfold = null_commentfold, rbool& warning = null_warning)
			: type         (type         )
			, font_name    (font_name    )
			, font_size    (font_size    )
			, font_fixed   (font_fixed   )
			, wordwrap     (wordwrap     )
			, horzscrollbar(horzscrollbar)
			, warning      (warning      )
			, bookmarkstyle(bookmarkstyle)
			, foldstyle    (foldstyle    )
			, commentfold  (commentfold  )
		{}

		~STYLEObject()
		{
			PropertyList::iterator it = properties.begin();
			while (it != properties.end())
			{
				delete *it++;
			};
		}
	};
	typedef  std::list<PTR(STYLEObject)>  ObjectList;

	class STYLEFont
	{
	public:
		tstring name;
		rbool   fixed;

		STYLEFont()
			: name (""  )
			, fixed(true)
		{}
		STYLEFont(CREF(tstring) name, rbool fixed = true)
			: name (name )
			, fixed(fixed)
		{}
	};
	typedef  std::list<STYLEFont>  FontList;

	ObjectList objects;
	FontList   fonts;
	rbool      isCurrentFixed;
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam);
	void loadFontsIntoCombo(rbool fixed = true);

	RDOStudioOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	STYLEProperty*    getCurrentProperty() const;
	STYLEObject*      getCurrentObject() const;
	STYLEObject::Type getCurrentObjectType() const;

	STYLEObject::Type previewAs;
	void setPreviewAsCombo(STYLEObject::Type type);

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

public:
	RDOStudioOptions();
	virtual ~RDOStudioOptions();

private:
	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdo::gui::tracer::LogStyle       style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	RDOStudioChartViewStyle          style_chart;
	RDOStudioFrameStyle              style_frame;

	RDOStudioOptionsGeneral*      general;
	RDOStudioOptionsEditor*       editor;
	RDOStudioOptionsTabs*         tabs;
	RDOStudioOptionsColorsStyles* styles;

	rdoEditor::RDOEditorEdit       preview_editor;
	rdoEditCtrl::RDOBuildEdit      preview_build;
	rdoEditCtrl::RDODebugEdit      preview_debug;
	rdo::gui::tracer::LogMainWnd   preview_trace;
	rdoEditor::RDOEditorResults    preview_results;
	rdoEditCtrl::RDOFindEdit       preview_find;

	rbool                          chart_need_delete;
	RDOStudioChartDoc*             preview_chart_doc;
	ChartView*                     preview_chart;
	std::vector<TracerTimeNow>     preview_times;
	TracerSerie                    preview_serie;

	RDOStudioFrameOptionsCtrl      preview_frame;

	typedef std::map<ruint,tstring> mapKeyAndUrl;
	mapKeyAndUrl                    m_keyAndUrl;

	tstring      resolveKeyAndUrl (ruint helpInfo);
	void         buildMap         ();
	void         onHelpButton();

	void updateStyles();
	void apply() const;
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);

	virtual BOOL ContinueModal();

	afx_msg BOOL OnHelpInfo(PTR(HELPINFO) pHelpInfo);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_OPTIONS_H_
