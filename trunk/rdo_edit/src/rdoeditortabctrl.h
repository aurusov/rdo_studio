#ifndef RDOEDITORTABCTRL_H
#define RDOEDITORTABCTRL_H
#pragma once

#include "rdotabctrl.h"
#include "rdoeditoredit.h"

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
enum RDOEditorTabItem {
	RDOEDIT_NONE = -1,
	RDOEDIT_PAT  = 0,
	RDOEDIT_RTP,
	RDOEDIT_RSS,
	RDOEDIT_OPR,
	RDOEDIT_FRM,
	RDOEDIT_FUN,
	RDOEDIT_DPT,
	RDOEDIT_SMR,
	RDOEDIT_PMD,
	RDOEDIT_PMV,
	RDOEDIT_TRC
};

class RDOEditorTabCtrl: public RDOTabCtrl
{
protected:

	CMenu popupMenu;

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorTabCtrl();
	virtual ~RDOEditorTabCtrl();

	RDOEditorTabItem getCurrentRDOItem() const;
	void setCurrentRDOItem( const RDOEditorTabItem item );
	virtual void changeCurrentItem();

	RDOEditorEdit* getCurrentEdit() const               { return (RDOEditorEdit*)getItemCurrent(); };
	RDOEditorEdit* getItemEdit( const int index ) const { return (RDOEditorEdit*)getItem( index ); };
};

#endif // RDOEDITORTABCTRL_H
