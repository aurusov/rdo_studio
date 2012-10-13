/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_docview.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      29.08.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_DOCVIEW_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_DOCVIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <afxole.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RPView
// --------------------------------------------------------------------------------
class RPFlowChart;
class RPObjectFlowChart;
class RPObjectClassInfo;

class RPView: public CWnd
{
private:
	RPFlowChart* flowchart;

public:
	RPView(QWidget* pParent);
	virtual ~RPView();

	QWidget* getQtParent();

protected:
	COleDropTarget target;
	const RPObjectClassInfo* getSrcClassInfo( COleDataObject* pDataObject ) const;

	virtual DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave();
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );

public:
	RPFlowChart* getFlowchart() { return flowchart; }
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void makeFlowChartWnd( RPObjectFlowChart* flowobj );

private:
	QWidget* m_pParent;

protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

class RPViewQt:
	public QWidget,
	public IInit
{
public:
	RPViewQt();
	virtual ~RPViewQt();

	RPView* getContext();

private:
	typedef  QWidget  parent_type;

	RPView*  m_pContext;
	CWnd     m_thisCWnd;

	void closeEvent (PTR(QCloseEvent)  event);
	void resizeEvent(PTR(QResizeEvent) event);

	DECLARE_IInit;
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_DOCVIEW_H_
