#ifndef CHATMAINFRM_H
#define CHATMAINFRM_H
#pragma once

#include "chatchildview.h"
#include "chatuserstreectrl.h"
#include "chatdockwnd.h"
#include "chatpopupmessage.h"
#include "chatstatusmode.h"

// ----------------------------------------------------------------------------
// ---------- CChatMainFrame
// ----------------------------------------------------------------------------
enum CChatCloseButtonAction {
	CCBA_Close    = 0,
	CCBA_Minimize = 1,
	CCBA_Tray     = 2
};

enum CChatMinimizeButtonAction {
	CCMA_Minimize = 0,
	CCMA_Tray     = 1
};

class CChatMainFrame: public CFrameWnd
{
protected: 
	CToolBar       statusModeToolBar;
	CStatusBar     statusBar;

	CFont defFont;

	CChatCloseButtonAction    closeButtonAction;
	CChatMinimizeButtonAction minimizeButtonAction;
	bool                      minimizeByClose;

	NOTIFYICONDATA trayIconData;
	UINT           trayTimer;
	bool           trayShowMessageIcon;
	bool           trayModifyLock;
	bool           trayRestorWndSize;
	bool           trayCloseApp;
	bool           useTray;
	bool           notifyAboutMessage;
	void initTray();
	void closeTray();
	void stopTrayTimer();

	bool   useAutoAway;
	int    autoAwayMinuts;
	bool   useAutoNotAvailible;
	int    autoNotAvailibleMinuts;
	UINT   awayTimer;
	CPoint prev_cursor;
	HWND   prev_wnd;
	int    idle_sec;
	bool   wasAutoChanged;
	CChatStatusModeType lastStatusMode;
	void startAutoStatusModeTimer();
	void stopAutoStatusModeTimer();

protected:
	//{{AFX_VIRTUAL(CChatMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnClose();
	afx_msg void OnHandleTrayNotify( UINT wParam, LONG lParam );
	afx_msg void OnTrayOpenHide();
	afx_msg void OnTrayCloseApp();
	afx_msg void OnUpdateTrayOpenHide( CCmdUI *pCmdUI );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected: 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

public:
	CChatMainFrame();
	virtual ~CChatMainFrame();

	CChatChildView     childView;
	CChatDockWnd       dock;
//	CChatUsersTreeCtrl* usersTreeView;

	CChatPopupMessage popupMessage;

	void modifyTray();

	void showPopupMessage( const std::string& value );

	bool isVisible() const;
	bool isActive();

	CChatCloseButtonAction getCloseButtonAction() const;
	void setCloseButtonAction( const CChatCloseButtonAction value );

	CChatMinimizeButtonAction getMinimizeButtonAction() const;
	void setMinimizeButtonAction( const CChatMinimizeButtonAction value );

	bool getUseTray() const;
	void setUseTray( const bool value );
	bool getNotifyAboutMessage() const;
	void setNotifyAboutMessage( const bool value );

	bool getUseAutoAway() const;
	void setUseAutoAway( const bool value );
	int  getAutoAwayMinuts() const;
	void setAutoAwayMinuts( const int minuts );
	bool getUseAutoNotAvailible() const;
	void setUseAutoNotAvailible( const bool value );
	int  getAutoNotAvailibleMinuts() const;
	void setAutoNotAvailibleMinuts( const int minuts );
	bool isAutoStatusMode() const;
	void restoreStatusMode();
	void resetAutoStatusMode();
};

#endif // CHATMAINFRM_H
