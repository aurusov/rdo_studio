#ifndef RDOSTUDIOOPTIONS_H
#define RDOSTUDIOOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdocolorcombobox.h>
#include "rdo_edit/rdoeditoredit.h"
#include "rdo_edit/rdoeditorresults.h"
#include "rdo_edit/rdoeditoreditstyle.h"
#include "rdo_edit/rdoeditorresultsstyle.h"
#include "edit_ctrls/rdobuildedit.h"
#include "edit_ctrls/rdodebugedit.h"
#include "edit_ctrls/rdofindedit.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdofindeditstyle.h"
#include "rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"
#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsEditor
// ----------------------------------------------------------------------------
class RDOStudioOptions;

class RDOStudioOptionsEditor: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

protected:
	//{{AFX_DATA(RDOStudioOptionsEditor)
	enum { IDD = IDD_OPTIONS_EDITOR };
	BOOL	m_bufferClearAuto;
	int		m_bufferDelay;
	BOOL	m_codecompUse;
	int		m_codecompShowFullList;
	BOOL	m_marginFold;
	BOOL	m_marginBookmark;
	BOOL	m_marginLineNumber;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioOptionsEditor)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsEditor)
	afx_msg void OnUseAutoCompleteCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateModify();
	afx_msg void OnClearAutoCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsEditor( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsEditor();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsTabs
// ----------------------------------------------------------------------------
class RDOStudioOptionsTabs: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

protected:
	//{{AFX_DATA(RDOStudioOptionsTabs)
	enum { IDD = IDD_OPTIONS_TABS };
	BOOL	m_tabUse;
	int		m_tabSize;
	BOOL	m_tabUseTabIndent;
	int		m_tabIndentSize;
	int		m_tabBackspaceUntabs;
	BOOL	m_tabAutoIndent;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioOptionsTabs)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsTabs)
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsTabs( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsTabs();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsStylesAndColors
// ----------------------------------------------------------------------------
class RDOStudioOptionsStylesAndColors: public CPropertyPage
{
private:

	class STYLEObject;

	class STYLEProperty {
	public:
		STYLEObject* object;
		std::string name;
		bool root;

		rdoEditCtrl::RDOFontStyle& font_style;

		STYLEProperty( STYLEObject* _object, std::string _name, bool _root, rdoEditCtrl::RDOFontStyle& _font_style ): object( _object ), name( _name ), root( _root ), font_style( _font_style ) {};
	};

	class STYLEFont {
	public:
		std::string name;
		bool fixed;

		STYLEFont(): name( "" ), fixed( true ) {};
		STYLEFont( std::string _name, bool _fixed = true ): name( _name ), fixed( _fixed ) {};
	};

	class STYLEObject {
	public:
		enum Type { none = 0, all, source, build, debug, tracer, results, find } type;
		std::string& font_name;
		int&         font_size;
		bool         font_fixed;
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

	std::list< STYLEObject* > objects;
	std::list< STYLEFont > fonts;
	bool isCurrentFixed;
	static int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam );
	void loadFontsIntoCombo( bool fixed = true );

	RDOStudioOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	STYLEProperty* getCurrentProperty() const;
	const STYLEObject* getCurrentObject() const;
	STYLEObject::Type getCurrentObjectType() const;

	STYLEObject::Type previewAs;
	void setPreviewAsCombo( STYLEObject::Type type );

	std::string all_font_name;
	int         all_font_size;
	rdoEditCtrl::RDOFontStyle all_font_style;
	void updatePropOfAllObject() const;

	rdoEditCtrl::RDOFontStyle null_font_style;

	void OnUpdateModify();

protected:
	//{{AFX_DATA(RDOStudioOptionsStylesAndColors)
	enum { IDD = IDD_OPTIONS_STYLESANDCOLORS };
	CButton	m_fontStyleUnderline;
	CButton	m_fontStyleItalic;
	CButton	m_fontStyleBold;
	CComboBox	m_fontSize;
	CComboBox	m_previewAs;
	CComboBox	m_fontName;
	CTreeCtrl	m_styleItem;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioOptionsStylesAndColors)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsStylesAndColors)
	virtual BOOL OnInitDialog();
	afx_msg void OnStyleItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPreviewAsChanged();
	afx_msg void OnFontNameChanged();
	afx_msg void OnFontSizeChanged();
	afx_msg void OnFontStyleBoldChanged();
	afx_msg void OnFontStyleItalicChanged();
	afx_msg void OnFontStyleUnderlineChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsStylesAndColors( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsStylesAndColors();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptions
// ----------------------------------------------------------------------------
class RDOStudioOptions: public CPropertySheet
{
friend class RDOStudioOptionsEditor;
friend class RDOStudioOptionsTabs;
friend class RDOStudioOptionsStylesAndColors;

private:
	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOLogEditStyle     style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
//	rdoEditCtrl::RDOBaseEditStyle    style_tracer;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;

	RDOStudioOptionsEditor*          editor;
	RDOStudioOptionsTabs*            tabs;
	RDOStudioOptionsStylesAndColors* styles;

	rdoEditor::RDOEditorEdit    preview_editor;
	rdoEditCtrl::RDOBuildEdit   preview_build;
	rdoEditCtrl::RDODebugEdit   preview_debug;
	RDOTracerLogCtrl            preview_tracer;
	rdoEditor::RDOEditorResults preview_results;
	rdoEditCtrl::RDOFindEdit    preview_find;

	void updateStyles();
	void apply() const;
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

protected:
	//{{AFX_VIRTUAL(RDOStudioOptions)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptions)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptions();
	virtual ~RDOStudioOptions();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOOPTIONS_H
