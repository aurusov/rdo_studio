#ifndef CHATSPLASH_H
#define CHATSPLASH_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSplash
// ----------------------------------------------------------------------------
class CChatSplash: public CWnd
{
private:
	std::string initInfo;
	CFont font;
	CProgressCtrl progress;
	bool useProgress;

public:
	CChatSplash();
	virtual ~CChatSplash();

	void show( CWnd* parent );
	void update();
	void setInitInfo( const std::string& str );
	void setInitInfo( const UINT resID );

	void setProgress( const int min = 0, const int max = 100, const int step = 1 );
	void stepProgress();
	void hideProgress();

protected:
	//{{AFX_VIRTUAL(CChatSplash)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatSplash)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSPLASH_H
