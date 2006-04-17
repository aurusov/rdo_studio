#ifndef RDO_PROCESS_TOOLBAR_H
#define RDO_PROCESS_TOOLBAR_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RPToolBar
// ----------------------------------------------------------------------------
class RPToolBar: public CToolBar
{
public:
	RPToolBar();
	virtual ~RPToolBar();

	COLORREF color_brush;
	COLORREF color_pen;
	COLORREF color_text;
	bool     empty_brush;
	bool     empty_pen;
	bool     empty_text;

protected:
	//{{AFX_VIRTUAL(RPToolBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPToolBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_TOOLBAR_H
