#ifndef RDOTABCTRL_H
#define RDOTABCTRL_H
#pragma once

#include <afxcmn.h>
#include <afxwin.h>

// ----------------------------------------------------------------------------
// ---------- RDOTab
// ----------------------------------------------------------------------------
class RDOTab: public CTabCtrl
{
friend class RDOTabCtrl;

protected:
	RDOTabCtrl* tabCtrl;

	RDOTab( RDOTabCtrl* _tabCtrl );
	virtual ~RDOTab();

	void showCurrentPage();
	void changeCurrentItem();

	int   getItemCount() const              { return GetItemCount();               };
	CWnd* getItem( const int index ) const;
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const bool direction = true, const bool loop = true ) const;
	CWnd* getItemCurrent() const            { return getItem( getCurrentIndex() ); };
	int   getCurrentIndex() const           { return GetCurSel();                  };

	void  insertItem( CWnd* pWnd, LPCTSTR lpName );

	void  setCurrentItem( const int index ) {
		if ( index < 0 && index > getItemCount() ) return;
		if ( index != getCurrentIndex() ) {
			SetCurSel( index );
			showCurrentPage();
			changeCurrentItem();
		}
	}
	int findItem( const CWnd* const item ) const {
		for ( int i = 0; i < getItemCount(); i++ ) {
			if ( getItem( i ) == item ) return i;
		}
		return -1;
	}
	int findItem( const HWND item ) const {
		for ( int i = 0; i < getItemCount(); i++ ) {
			if ( getItem( i )->m_hWnd == item ) return i;
		}
		return -1;
	}

	//{{AFX_MSG(RDOTab)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult ) {
		showCurrentPage();
		changeCurrentItem();
	}
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOTab)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
};

// ----------------------------------------------------------------------------
// ---------- RDOTabCtrl
// ----------------------------------------------------------------------------
class RDOTabCtrl: public CWnd
{
DECLARE_DYNAMIC( RDOTabCtrl )
friend class RDOTab;

protected:
	RDOTab* tab;
	bool autoDelete;

	virtual void changeCurrentItem() {};

	//{{AFX_MSG(RDOTabCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnEnable( BOOL bEnable );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOTabCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	RDOTabCtrl();
	virtual ~RDOTabCtrl();

	int   getItemCount() const             { return tab->getItemCount();   }
	CWnd* getItem( const int index ) const { return tab->getItem( index ); }
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const bool direction = true, const bool loop = true ) const {
		return tab->getItemNext( currentItem, direction, loop );
	}
	CWnd* getItemCurrent() const                   { return tab->getItemCurrent();    }
	int   getCurrentIndex() const                  { return tab->getCurrentIndex();   }
	int   findItem( const CWnd* const item ) const { return tab->findItem( item );    }
	void  insertItem( CWnd* pWnd, LPCTSTR lpName ) { tab->insertItem( pWnd, lpName ); }
	void  setCurrentItem( const int index )        { tab->setCurrentItem( index );    }

	void setAutoDelete( const bool value )         { autoDelete = value;              }

	void modifyTabStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 ) {
		tab->ModifyStyle( dwRemove, dwAdd, nFlags );
	}

	CWnd* getTabAsParent() const { return reinterpret_cast<CWnd*>(tab); };
};

#endif // RDOTABCTRL_H
