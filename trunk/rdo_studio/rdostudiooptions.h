#ifndef RDOSTUDIOOPTIONS_H
#define RDOSTUDIOOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdocolorcombobox.h>
#include "rdo_edit/rdoeditoredit.h"
#include "rdo_edit/rdoeditoreditstyle.h"
#include "rdo_edit/rdoeditorresultsstyle.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdofindeditstyle.h"
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
	RDOStudioOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

protected:
	//{{AFX_DATA(RDOStudioOptionsStylesAndColors)
	enum { IDD = IDD_OPTIONS_STYLESANDCOLORS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioOptionsStylesAndColors)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsStylesAndColors)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;

	RDOStudioOptionsEditor*          editor;
	RDOStudioOptionsTabs*            tabs;
	RDOStudioOptionsStylesAndColors* styles;

	rdoEditor::RDOEditorEdit edit;

	void apply();
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
