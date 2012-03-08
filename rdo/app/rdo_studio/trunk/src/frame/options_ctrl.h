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

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // RDOSTUDIOFRAMEOPTIONSCTRL_H
