#ifndef RDOEDITORTABCTRL_H
#define RDOEDITORTABCTRL_H
#pragma once

#include "rdo_studio/rdo_edit/rdoeditoredit.h"
#include "rdo_ctrls/rdotabctrl.h"

class RDOStudioEditBaseView;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
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

	template <class T>
	PTR(T) getCurrentWnd() const
	{
		return dynamic_cast<PTR(T)>(getItemCurrent());
	}

	template <class T>
	PTR(T) getItemWnd(const int index) const
	{
		return dynamic_cast<PTR(T)>(getItem(index));
	}

	template <class T>
	PTR(T) getItemWnd(const rdoModelObjects::RDOFileType type) const
	{
		int index = typeToIndex(type);
		return index != -1 ? getItemWnd<T>(index) : NULL;
	}
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORTABCTRL_H
