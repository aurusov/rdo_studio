/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreectrl.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdotreectrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreeitem.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTreeCtrl
// --------------------------------------------------------------------------------
class RDODropSource : public COleDropSource
{
protected:

	//{{AFX_VIRTUAL(RDODropSource)
	virtual SCODE GiveFeedback( DROPEFFECT dropEffect );
	//}}AFX_VIRTUAL
public:
	RDODropSource() : COleDropSource() {};
	virtual ~RDODropSource() {};
};

class RDOTracerResType;
class RDOTracerResource;
class RDOTracerPattern;
class RDOTracerOperationBase;
class RDOTracerResult;

class RDOTracerTreeCtrl: public RDOTreeCtrl  
{
DECLARE_DYNCREATE( RDOTracerTreeCtrl )
private:
	CImageList imageList;

	RDOTracerTreeItem rootItem;
	RDOTracerTreeItem rtpItem;
	RDOTracerTreeItem patItem;
	RDOTracerTreeItem pmvItem;

	void setHasChildren( const RDOTracerTreeItem* item, const bool hasChildren = true );

protected:
	COleDataSource source;
	RDODropSource  dropsource;

	RDOTracerTreeItem* getIfItemIsDrawable( const HTREEITEM hItem ) const;

	void doDragDrop( RDOTracerTreeItem* item, CPoint point );

	CMenu popupMenu;

	void addToNewChart( const HTREEITEM hitem ) const;
	bool findInCharts( const HTREEITEM hitem ) const;

	//{{AFX_VIRTUAL(RDOTracerTreeCtrl)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerTreeCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnAddToNewChart();
	afx_msg void OnUpdateAddToNewChart( CCmdUI* pCmdUI );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateChartFindincharts(CCmdUI* pCmdUI);
	afx_msg void OnChartFindincharts();
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOTracerTreeCtrl();
	virtual ~RDOTracerTreeCtrl();

	BOOL setModelName( const std::string& modelName );
	void addResourceType( RDOTracerResType* resType );
	void addResource( RDOTracerResource* res );
	void updateResource( RDOTracerResource* const res );
	void addPattern( RDOTracerPattern* pat );
	void addOperation( RDOTracerOperationBase* opr );
	//void addIrregularEvent( RDOTracerOperation* opr );
	void addResult( RDOTracerResult* res );
	void deleteChildren( const RDOTracerTreeItem* parent );
	void clear();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_
