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

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHelpKeyword();
	DECLARE_MESSAGE_MAP()
};

#endif // RDOSTUDIOFRAMETREECTRL_H
