#ifndef RDOSTUDIOOPTIONS_H
#define RDOSTUDIOOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_edit/rdoeditoreditstyle.h"
#include "rdo_edit/rdoeditorresultsstyle.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdofindeditstyle.h"
#include <rdocolorcombobox.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsEditor
// ----------------------------------------------------------------------------
class RDOStudioOptions;

class RDOStudioOptionsEditor: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

	int useAutoComplete;
	int showFullList;

protected:
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
	//{{AFX_VIRTUAL(RDOStudioOptionsTabs)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsTabs)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsTabs( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsTabs();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsColorsAndStyles
// ----------------------------------------------------------------------------
class RDOStudioOptionsColorsAndStyles: public CPropertyPage
{
private:
	RDOStudioOptions* sheet;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

protected:
	//{{AFX_VIRTUAL(RDOStudioOptionsColorsAndStyles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsColorsAndStyles)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsColorsAndStyles( RDOStudioOptions& _sheet );
	virtual ~RDOStudioOptionsColorsAndStyles();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptions
// ----------------------------------------------------------------------------
class RDOStudioOptions: public CPropertySheet
{
friend class RDOStudioOptionsEditor;
friend class RDOStudioOptionsTabs;
friend class RDOStudioOptionsColorsAndStyles;

private:
	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOLogEditStyle     style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;

	RDOStudioOptionsEditor*          editor;
	RDOStudioOptionsTabs*            tabs;
	RDOStudioOptionsColorsAndStyles* colorsAndStyles;

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
