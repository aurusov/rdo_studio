#ifndef CHATPOPUPMESSAGE_H
#define CHATPOPUPMESSAGE_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- CChatPopupMessage
// ----------------------------------------------------------------------------
enum CChatPopupPosition {
	CPP_RightBottom = 0,
	CPP_RightTop,
	CPP_LeftBottom,
	CPP_LeftTop
};

class CChatPopupMessage: public CDialog
//class CChatPopupMessage: public CWnd
{
protected:
	std::string message;

	UINT popupTimer;

	bool               usePopup;
	CChatPopupPosition position;
	int                timerDelay;
	COLORREF           textColor;
	COLORREF           backgroundColor;

	void showMessage();

public:
	CChatPopupMessage();
	virtual ~CChatPopupMessage();

	void setMessage( const std::string& value );

	bool getUsePopup() const               { return usePopup; }
	void setUsePopup( const bool value );

	CChatPopupPosition getPosition() const { return position; }
	void setPosition( const CChatPopupPosition value );

	int getTimerDelay() const              { return timerDelay; }
	void setTimerDelay( const int value );

	COLORREF getTextColor() const          { return textColor; }
	void setTextColor( const COLORREF value );

	COLORREF getBackgroundColor() const    { return backgroundColor; }
	void setBackgroundColor( const COLORREF value );

protected:
	//{{AFX_VIRTUAL(CChatPopupMessage)
	protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatPopupMessage)
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
//	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATPOPUPMESSAGE_H
