#ifndef RDOSTUDIOOPTIONS_H
#define RDOSTUDIOOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_edit/rdoeditoreditstyle.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdofindeditstyle.h"
#include <rdocolorcombobox.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsSourceEditor
// ----------------------------------------------------------------------------
class RDOStudioOptionsSourceEditor: public CPropertyPage
{
protected:
	//{{AFX_VIRTUAL(RDOStudioOptionsSourceEditor)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsSourceEditor)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsSourceEditor();
	virtual ~RDOStudioOptionsSourceEditor();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsStyleColor
// ----------------------------------------------------------------------------
class RDOStudioOptionsStyleColor: public CPropertyPage
{
private:
	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

protected:
	//{{AFX_VIRTUAL(RDOStudioOptionsStyleColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOptionsStyleColor)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptionsStyleColor();
	virtual ~RDOStudioOptionsStyleColor();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptions
// ----------------------------------------------------------------------------
class RDOStudioOptions: public CPropertySheet
{
private:
	rdoEditor::RDOEditorEditStyle editorStyle;
	RDOLogEditStyle               buildStyle;
	rdoBaseEdit::RDOBaseEditStyle debugStyle;
	rdoEditor::RDOEditorEditStyle resultsStyle;
	RDOFindEditStyle              findStyle;

	RDOStudioOptionsSourceEditor* sourceEditor;
	RDOStudioOptionsStyleColor*   styleColor;

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
