#ifndef RDOTABCTRL_H
#define RDOTABCTRL_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTab
// ----------------------------------------------------------------------------
class RDOTab : public CTabCtrl
{
friend class RDOTabCtrl;
protected:
	RDOTab();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

// ----------------------------------------------------------------------------
// ---------- RDOTabCtrl
// ----------------------------------------------------------------------------
class RDOTabCtrl: public CWnd
{
protected:
	RDOTab tab;
	bool autoDelete;

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	void showCurrentPage();

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnEnable( BOOL bEnable );
	afx_msg void OnSelChangeTab( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

public:
	RDOTabCtrl();
	virtual ~RDOTabCtrl();

	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

	int   getItemCount() const              { return tab.GetItemCount();           };
	CWnd* getItem( const int index ) const;
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const bool direction = true, const bool loop = true ) const;
	CWnd* getItemCurrent() const            { return getItem( getCurrentIndex() ); };
	int   getCurrentIndex() const           { return tab.GetCurSel();              };
	int   findItem( const CWnd* const item ) const;
	void  insertItem( CWnd* pWnd, LPCTSTR lpName );
	void  setCurrentItem( const int index );
	virtual void changeCurrentItem();

	void setAutoDelete( const bool value ) { autoDelete = value;  };
	void setFont( CFont* font )            { tab.SetFont( font ); };
};

#endif // RDOTABCTRL_H
