#include <vector>

#pragma once

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint ();
	afx_msg void OnTimer (UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	typedef std::vector<Gdiplus::Image*> ImageList;
	typedef ImageList::const_iterator    ImageIt;

	ImageList m_imageList;
	ImageIt   m_imageCurrent;
};
