#ifndef RDOSTUDIOOPTIONS_H
#define RDOSTUDIOOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdocolorcombobox.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioSourceEditorOptions
// ----------------------------------------------------------------------------
class RDOStudioSourceEditorOptions: public CPropertyPage
{
public:
	RDOStudioSourceEditorOptions();
	virtual ~RDOStudioSourceEditorOptions();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioStyleColorOptions
// ----------------------------------------------------------------------------
class RDOStudioStyleColorOptions: public CPropertyPage
{
private:
	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

public:
	RDOStudioStyleColorOptions();
	virtual ~RDOStudioStyleColorOptions();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptions
// ----------------------------------------------------------------------------
class RDOStudioOptions: public CPropertySheet
{
//friend class RDOStudioEditorOptions;
//friend class RDOStudioColorsOptions;

protected:
//	RDOStudioEditStyle prevStyle;

	RDOStudioSourceEditorOptions* sourceEditorOptions;
	RDOStudioStyleColorOptions*   styleColorOptions;

	void apply();
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

	//{{AFX_MSG(RDOStudioOutput)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioOptions();
	virtual ~RDOStudioOptions();

//	RDOEditorEditStyle editorStyle;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOOPTIONS_H
