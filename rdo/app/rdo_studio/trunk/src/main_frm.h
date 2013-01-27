/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_frm.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MAIN_FRM_H_
#define _RDO_STUDIO_MFC_MAIN_FRM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
#include <QtGui/qmainwindow.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/status_bar.h"
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/main_window_ui.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOToolBar
// --------------------------------------------------------------------------------
class RDOToolBar: public CToolBar
{
protected:
	CImageList disabledImage;

public:
	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID );
};

// --------------------------------------------------------------------------------
// -------------------- RDOToolBarModel
// --------------------------------------------------------------------------------
class RDOToolBarModel: public RDOToolBar
{
protected:
	double log101;
	CSliderCtrl slider;

	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	DECLARE_MESSAGE_MAP()

public:
	RDOToolBarModel(): RDOToolBar(), log101( log(101.0) ) {}

	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID );

	double getSpeed() const { return log( double(slider.GetPos() + 1) ) / log101; }
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioMainFrame
// --------------------------------------------------------------------------------
class RDOStudioMainFrame
	: public QMainWindow
	, public MainWindowBase
	, private Ui::MainWindow
{
Q_OBJECT

friend class RDOToolBar;
friend class RDOStudioModelDoc;

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

	void init();

	virtual void updateAllStyles() const;

	virtual void showWorkspace();
	virtual void showOutput   ();

	virtual void setVisible(rbool visible);

	double getSpeed() const { return modelToolBar.getSpeed(); }

	void beginProgress( const int lower = 0, const int upper = 100, const int step = 1 );
	void getProgressRange( int& lower, int& upper ) const  { statusBar.getRange( lower, upper ); };
	void setProgress( const int pos )                      { statusBar.setPos( pos );            };
	int  getProgress() const                               { return statusBar.getPos();          };
	void offsetProgress( const int offset )                { statusBar.offsetPos( offset );      };
	void stepProgress()                                    { statusBar.stepIt();                 };
	void endProgress();

	void update_start();
	void update_stop();

	PTR(CWnd) c_wnd() { return &m_thisCWnd; }

private:
	typedef  QMainWindow  parent_type;

	CWnd                    m_thisCWnd;
	RDOToolBar              fileToolBar;
	RDOToolBar              editToolBar;
	RDOToolBar              zoomToolBar;
	RDOToolBarModel         modelToolBar;
	RDOStudioStatusBar      statusBar;

	virtual void closeEvent(QCloseEvent* event);
	virtual void showEvent (QShowEvent*  event);
	virtual void hideEvent (QHideEvent*  event);

	UINT m_updateTimer;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFileToolbar();
	afx_msg void OnViewEditToolbar();
	afx_msg void OnViewZoomToolbar();
	afx_msg void OnViewModelToolbar();
	afx_msg void OnUpdateViewFileToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEditToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewModelToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewOptions();
	afx_msg void OnHelpKeyword();
	afx_msg void OnModelRuntimeMaxSpeed();
	afx_msg void OnModelRuntimeJump();
	afx_msg void OnModelRuntimeSync();
	afx_msg void OnModelRuntimePause();
	afx_msg void OnUpdateModelRuntimeMaxSpeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimeJump(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimeSync(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimePause(CCmdUI* pCmdUI);
	afx_msg void OnModelShowRateInc();
	afx_msg void OnModelShowRateIncFour();
	afx_msg void OnModelShowRateDecFour();
	afx_msg void OnModelShowRateDec();
	afx_msg void OnUpdateModelShowRateInc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDec(CCmdUI* pCmdUI);
	afx_msg void OnModelFrameNext();
	afx_msg void OnModelFramePrev();
	afx_msg void OnUpdateModelFrameNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelFramePrev(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelSpeedStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnWorkspaceShow();
	afx_msg void OnOutputShow();
	afx_msg void OnMethodCommandRange( UINT id );
	afx_msg void OnMethodUpdateRange( CCmdUI* pCmdUI );
};

#endif // _RDO_STUDIO_MFC_MAIN_FRM_H_
