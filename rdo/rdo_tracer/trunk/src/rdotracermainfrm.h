#ifndef RDOTRACERMAINFRM_H
#define RDOTRACERMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "./tracer_ctrls/rdosplitterwnd.h"
//#include "rdotracertrace.h"
#include "./trace_files/rdotracer.h"
#include "./tracer_ctrls/rdotracerlogctrl.h"
//#include "rdotracertreectrl.h"
#include "./tracer_ctrls/rdotracertreectrl.h"
//#include "rdotracertabctrl.h"
#include <rdotabctrl.h>
#include "./tracer_ctrls/rdotracerlogstyle.h"
#include "./trace_files/rdostudiochartviewstyle.h"
#include "rdotracerpane.h"

#include "rdotracerfilereader.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerMainFrame
// ----------------------------------------------------------------------------
enum ShowMode
{
	SM_NoShow,
	SM_Monitor,
	SM_Animation
};

class RDOTracerPane;

class RDOTracerMainFrame : public CFrameWnd
{
private:
	rdoTracerLog::RDOTracerLogCtrl*   log;
	RDOTabCtrl*         tab;
	RDOTracerTreeCtrl*  tree;

	RDOTracerPane* log_pane;
	RDOTracerPane* tab_pane;
	RDOTracerPane* tree_pane;

protected:
	CStatusBar   statusBar;
	
	CToolBar   traceToolBar;
	CImageList traceToolBarImageList;

	CToolBar   logToolBar;
	CImageList logToolBarImageList;

	CToolBar   zoomToolBar;
	CImageList zoomToolBarImageList;
	
	RDOSplitterWnd logSplitter;
	RDOSplitterWnd treeSplitter;
	
	CFont defFont;

	ShowMode showMode;

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

	//{{AFX_VIRTUAL(RDOTracerMainFrame)
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerMainFrame)
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnMove( int x, int y );
	afx_msg void onStartTrace();
	afx_msg void onStopTrace();
	afx_msg void onViewLog()          { log_pane->GetStyle() & WS_VISIBLE ? logSplitter.hideRow( 1 ) : logSplitter.showRow();          };
	afx_msg void onViewTree()         { tree_pane->GetStyle() & WS_VISIBLE ? treeSplitter.hideColumn( 0 ) : treeSplitter.showColumn(); };
	afx_msg void onViewTraceToolBar() { ShowControlBar( &traceToolBar, !(traceToolBar.GetStyle() & WS_VISIBLE), false );          };
	afx_msg void onViewLogToolBar()   { ShowControlBar( &logToolBar, !(logToolBar.GetStyle() & WS_VISIBLE), false );              };
	afx_msg void onViewZoomToolBar()  { ShowControlBar( &zoomToolBar, !(zoomToolBar.GetStyle() & WS_VISIBLE), false );            };
	afx_msg void onViewStatusBar()    { ShowControlBar( &statusBar, !(statusBar.GetStyle() & WS_VISIBLE), false );                };
	afx_msg void onViewOptions();
	afx_msg void onUpdateStartTrace( CCmdUI *pCmdUI )   { pCmdUI->Enable( !trace_reader.isTracing() );                    };
	afx_msg void onUpdateStopTrace( CCmdUI *pCmdUI )    { pCmdUI->Enable( trace_reader.isTracing() );                     };
	afx_msg void onUpdateLog( CCmdUI *pCmdUI )          { pCmdUI->SetCheck( log_pane->GetStyle() & WS_VISIBLE );         };
	afx_msg void onUpdateTree( CCmdUI *pCmdUI )         { pCmdUI->SetCheck( tree_pane->GetStyle() & WS_VISIBLE );        };
	afx_msg void onUpdateTraceToolBar( CCmdUI *pCmdUI ) { pCmdUI->SetCheck( traceToolBar.GetStyle() & WS_VISIBLE ); };
	afx_msg void onUpdateLogToolBar( CCmdUI *pCmdUI )   { pCmdUI->SetCheck( logToolBar.GetStyle() & WS_VISIBLE );   };
	afx_msg void onUpdateZoomToolBar( CCmdUI *pCmdUI )  { pCmdUI->SetCheck( zoomToolBar.GetStyle() & WS_VISIBLE );  };
	afx_msg void onUpdateStatusBar( CCmdUI *pCmdUI )    { pCmdUI->SetCheck( statusBar.GetStyle() & WS_VISIBLE );    };
	afx_msg void OnSmNoshow();
	afx_msg void OnUpdateSmNoshow(CCmdUI* pCmdUI);
	afx_msg void OnSmAnimation();
	afx_msg void OnUpdateSmAnimation(CCmdUI* pCmdUI);
	afx_msg void OnSmMonitor();
	afx_msg void OnUpdateSmMonitor(CCmdUI* pCmdUI);
	afx_msg void OnModelDelayInc();
	afx_msg void OnUpdateModelDelayInc(CCmdUI* pCmdUI);
	afx_msg void OnModelDelayIncfour();
	afx_msg void OnUpdateModelDelayIncfour(CCmdUI* pCmdUI);
	afx_msg void OnModelDelayDecfour();
	afx_msg void OnUpdateModelDelayDecfour(CCmdUI* pCmdUI);
	afx_msg void OnModelDelayDec();
	afx_msg void OnUpdateModelDelayDec(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateDelayStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

protected: 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

public:
	RDOTracerMainFrame();
	virtual ~RDOTracerMainFrame();
	rdoTracerLog::RDOTracerLogCtrl*   getLog() const;
	RDOTabCtrl*        getTab() const;
	RDOTracerTreeCtrl* getTree() const;
	void setModelName( const std::string modelName );
	ShowMode getShowMode() { return showMode; };

	rdoTracerLog::RDOTracerLogStyle  style_trace;
	RDOStudioChartViewStyle          style_chart;
	void updateAllStyles() const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOTRACERMAINFRM_H
