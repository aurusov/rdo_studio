/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditortabctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "ui/mfc_ctrls/rdotabctrl.h"
// --------------------------------------------------------------------------------

class RDOStudioEditBaseView;

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorTabCtrl
// --------------------------------------------------------------------------------
class RDOEditorTabCtrl: public RDOTabCtrl
{
private:
	RDOStudioEditBaseView*        view;
	rdoEditCtrl::RDOBaseEditGroup group;

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

	rdoModelObjects::RDOFileType indexToType( const int index ) const;
	int typeToIndex( const rdoModelObjects::RDOFileType type ) const;
	bool typeSupported( const rdoModelObjects::RDOFileType type ) const { return typeToIndex( type ) != -1;        }

	rdoModelObjects::RDOFileType getCurrentRDOItem() const              { return indexToType( getCurrentIndex() ); }
	void setCurrentRDOItem( const rdoModelObjects::RDOFileType type );

	RDOEditorEdit* getCurrentEdit() const               { return static_cast<RDOEditorEdit*>(getItemCurrent()); }
	RDOEditorEdit* getItemEdit( const int index ) const { return static_cast<RDOEditorEdit*>(getItem( index )); }
	RDOEditorEdit* getItemEdit( const rdoModelObjects::RDOFileType type ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_
