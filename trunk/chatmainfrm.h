#ifndef CHATMAINFRM_H
#define CHATMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatchildview.h"
#include "chatdock.h"
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
private:
	CToolBar       mainToolBar;
	CImageList     mainToolBarImageList;
	CToolBar       editToolBar;
	CImageList     editToolBarImageList;
	CToolBar       statusModeToolBar;
	CImageList     statusModeToolBarImageList;
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

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

public:
	CChatMainFrame();
	virtual ~CChatMainFrame();

	CChatChildView  childView;
	CChatDock       dock;

	CChatPopupMessage popupMessage;

	void modifyTray();

	void showPopupMessage( const std::string& value );

	bool isVisible() const;
	bool isActive();

	CChatCloseButtonAction getCloseButtonAction() const              { return closeButtonAction;    }
	void setCloseButtonAction( const CChatCloseButtonAction value );

	CChatMinimizeButtonAction getMinimizeButtonAction() const        { return minimizeButtonAction; }
	void setMinimizeButtonAction( const CChatMinimizeButtonAction value );

	bool getUseTray() const               { return useTray;            }
	void setUseTray( const bool value );
	bool getNotifyAboutMessage() const    { return notifyAboutMessage; }
	void setNotifyAboutMessage( const bool value );

	bool getUseAutoAway() const                         { return useAutoAway;            }
	void setUseAutoAway( const bool value );
	int  getAutoAwayMinuts() const                      { return autoAwayMinuts;         }
	void setAutoAwayMinuts( const int minuts );
	bool getUseAutoNotAvailible() const                 { return useAutoNotAvailible;    }
	void setUseAutoNotAvailible( const bool value );
	int  getAutoNotAvailibleMinuts() const              { return autoNotAvailibleMinuts; }
	void setAutoNotAvailibleMinuts( const int minuts );
	bool isAutoStatusMode() const                       { return wasAutoChanged;         }
	void restoreStatusMode();
	void resetAutoStatusMode();

	void sendMessageToUser() { OnUserSendMessage(); }

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

	//{{AFX_VIRTUAL(CChatMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnClose();
	afx_msg void OnTrayOpenHide();
	afx_msg void OnTrayCloseApp();
	afx_msg void OnUpdateTrayOpenHide( CCmdUI *pCmdUI );
	afx_msg void OnFileRefreshUserList();
	afx_msg void OnFileRefreshNetwork();
	afx_msg void OnUpdateFileRefreshNetwork(CCmdUI* pCmdUI);
	afx_msg void OnViewDockWindow();
	afx_msg void OnUpdateViewDockWindow(CCmdUI* pCmdUI);
	afx_msg void OnViewMainToolbar();
	afx_msg void OnUpdateViewMainToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewEditToolbar();
	afx_msg void OnUpdateViewEditToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusModeToolbar();
	afx_msg void OnUpdateViewStatusModeToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnSound();
	afx_msg void OnUpdateSound(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusMode( CCmdUI* pCmdUI );
	afx_msg void OnUpdateStatusModeInfo( CCmdUI* pCmdUI );
	afx_msg void OnToCryOut();
	afx_msg void OnOptions();
	afx_msg void OnUserSendMessage();
	afx_msg void OnUpdateUserSendMessage(CCmdUI* pCmdUI);
	afx_msg void OnUserIngnore();
	afx_msg void OnUpdateUserIngnore(CCmdUI* pCmdUI);
	afx_msg void OnHelpContents();
	afx_msg void OnHelpAbout();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnShowUserList();
	afx_msg void OnShowNetwork();
	afx_msg void OnShowSmiles();
	afx_msg void OnShowEdit();
	//}}AFX_MSG
	afx_msg void OnHandleTrayNotify( UINT wParam, LONG lParam );
	afx_msg void OnStatusMode( UINT nID );
	afx_msg void OnStatusModeInfo( UINT nID );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATMAINFRM_H
