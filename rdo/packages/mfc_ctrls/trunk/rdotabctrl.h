/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotabctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _UI_MFC_CTRLS_RDOTABCTRL_H_
#define _UI_MFC_CTRLS_RDOTABCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <afxcmn.h>
#include <afxwin.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTab
// --------------------------------------------------------------------------------
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
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const rbool direction = true, const rbool loop = true ) const;
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

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
	{
		UNUSED(pNMHDR );
		UNUSED(pResult);

		showCurrentPage  ();
		changeCurrentItem();
	}
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOTabCtrl
// --------------------------------------------------------------------------------
class RDOTabCtrl: public CWnd
{
DECLARE_DYNAMIC( RDOTabCtrl )
friend class RDOTab;

protected:
	RDOTab* tab;
	rbool autoDelete;

	virtual void changeCurrentItem() {};

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );

private:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnEnable( BOOL bEnable );
	DECLARE_MESSAGE_MAP()

public:
	RDOTabCtrl();
	virtual ~RDOTabCtrl();

	int   getItemCount() const             { return tab->getItemCount();   }
	CWnd* getItem( const int index ) const { return tab->getItem( index ); }
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const rbool direction = true, const rbool loop = true ) const {
		return tab->getItemNext( currentItem, direction, loop );
	}
	CWnd* getItemCurrent() const                   { return tab->getItemCurrent();    }
	int   getCurrentIndex() const                  { return tab->getCurrentIndex();   }
	int   findItem( const CWnd* const item ) const { return tab->findItem( item );    }
	void  insertItem( CWnd* pWnd, LPCTSTR lpName ) { tab->insertItem( pWnd, lpName ); }
	void  setCurrentItem( const int index )        { tab->setCurrentItem( index );    }

	void setAutoDelete( const rbool value )        { autoDelete = value;              }

	void modifyTabStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 ) {
		tab->ModifyStyle( dwRemove, dwAdd, nFlags );
	}

	CWnd* getTabAsParent() const { return reinterpret_cast<CWnd*>(tab); };
};

#endif // _UI_MFC_CTRLS_RDOTABCTRL_H_
