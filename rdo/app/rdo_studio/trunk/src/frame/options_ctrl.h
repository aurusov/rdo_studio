#ifndef RDOSTUDIOFRAMEOPTIONSCTRL_H
#define RDOSTUDIOFRAMEOPTIONSCTRL_H

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameOptionsCtrl
// --------------------------------------------------------------------------------
class RDOStudioFrameStyle;

class RDOStudioFrameOptionsCtrl: public CWnd
{
private:
	CBitmap bmp;
	CFont   font;
	RDOStudioFrameStyle* style;

public:
	RDOStudioFrameOptionsCtrl();
	virtual ~RDOStudioFrameOptionsCtrl();

	void setStyle( RDOStudioFrameStyle* _style );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioFrameOptionsCtrl)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioFrameOptionsCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEOPTIONSCTRL_H
