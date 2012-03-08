#ifndef RDOSTUDIOFRAMETREECTRL_H
#define RDOSTUDIOFRAMETREECTRL_H

#include "ui/mfc_ctrls/rdotreectrl.h"

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTreeCtrl
// --------------------------------------------------------------------------------
class RDOStudioFrameTreeCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

public:
	RDOStudioFrameTreeCtrl();
	virtual ~RDOStudioFrameTreeCtrl();

	void expand();

protected:
	//{{AFX_VIRTUAL(RDOStudioFrameTreeCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioFrameTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMETREECTRL_H
