#ifndef CHATPOPUPMESSAGE_H
#define CHATPOPUPMESSAGE_H
#pragma once

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
	CString message;

	UINT popupTimer;

	bool               usePopup;
	CChatPopupPosition position;
	int                timerDelay;
	COLORREF           textColor;
	COLORREF           backgroundColor;

	void showMessage();

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

public:
	CChatPopupMessage();
	virtual ~CChatPopupMessage();

	void setMessage( const CString& value );

	bool getUsePopup() const;
	void setUsePopup( const bool value );

	CChatPopupPosition getPosition() const;
	void setPosition( const CChatPopupPosition value );

	int getTimerDelay() const;
	void setTimerDelay( const int value );

	COLORREF getTextColor() const;
	void setTextColor( const COLORREF value );

	COLORREF getBackgroundColor() const;
	void setBackgroundColor( const COLORREF value );
};

#endif // CHATPOPUPMESSAGE_H
