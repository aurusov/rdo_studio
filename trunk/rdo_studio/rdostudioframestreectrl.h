#ifndef RDOSTUDIOFRAMESTREECTRL_H
#define RDOSTUDIOFRAMESTREECTRL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdotreectrl.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioFramesTreeCtrl
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc;
class RDOStudioFrameView;

class RDOStudioFramesTreeCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

	class TreeCtrlItem {
	friend class RDOStudioFramesTreeCtrl;
	public:
		TreeCtrlItem(): hitem( 0 ), doc( NULL ), view( NULL ) {};
		HTREEITEM           hitem;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
	};
	static std::vector< TreeCtrlItem* > frames;

public:
	RDOStudioFramesTreeCtrl();
	virtual ~RDOStudioFramesTreeCtrl();

	void insertItem( const std::string& name );
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const  { return frames[index]->doc;  };
	RDOStudioFrameView* getFrameView( const int index ) const { return frames[index]->view; };
	void clear();

	void expand();

protected:
	//{{AFX_VIRTUAL(RDOStudioFramesTreeCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioFramesTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMESTREECTRL_H
