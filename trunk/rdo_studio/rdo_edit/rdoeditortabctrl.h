#ifndef RDOEDITORTABCTRL_H
#define RDOEDITORTABCTRL_H
#pragma once

#include <rdotabctrl.h>

#include "rdoeditoredit.h"
#include "rdoeditoreditstyle.h"

class RDOStudioEditBaseView;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
enum RDOEditorTabItem {
	RDOEDIT_PAT = 0,
	RDOEDIT_RTP,
	RDOEDIT_RSS,
	RDOEDIT_OPR,
	RDOEDIT_FRM,
	RDOEDIT_FUN,
	RDOEDIT_DPT,
	RDOEDIT_SMR,
	RDOEDIT_PMD
};

class RDOEditorTabCtrl: public RDOTabCtrl
{
private:
	RDOStudioEditBaseView* view;
	RDOEditorEditStyle     style;
	RDOEditorSciEditList   group;

protected:
	//{{AFX_MSG(RDOEditorTabCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorTabCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	RDOEditorTabCtrl( RDOStudioEditBaseView* _view );
	virtual ~RDOEditorTabCtrl();

	RDOEditorTabItem getCurrentRDOItem() const;
	void setCurrentRDOItem( const RDOEditorTabItem item );

	RDOEditorEdit* getCurrentEdit() const               { return (RDOEditorEdit*)getItemCurrent(); };
	RDOEditorEdit* getItemEdit( const int index ) const { return (RDOEditorEdit*)getItem( index ); };
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORTABCTRL_H
