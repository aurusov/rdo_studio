/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view_base.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/src/edit/document_base.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseView
// --------------------------------------------------------------------------------
#define timerBuf1_ID 1
#define timerBuf2_ID 2
#define timerBuf3_ID 3
#define timerBuf4_ID 4

IMPLEMENT_DYNCREATE(RDOStudioEditBaseView, RDOStudioView)

BEGIN_MESSAGE_MAP(RDOStudioEditBaseView, RDOStudioView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_PRINT, RDOStudioView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioEditBaseView::RDOStudioEditBaseView():
	RDOStudioView(),
	buf1( "" ),
	buf2( "" ),
	buf3( "" ),
	buf4( "" ),
	resetBuf1( true ),
	resetBuf2( true ),
	resetBuf3( true ),
	resetBuf4( true ),
	currentBuffer( 1 ),
	timerBuf1( 0 ),
	timerBuf2( 0 ),
	timerBuf3( 0 ),
	timerBuf4( 0 )
{
}

RDOStudioEditBaseView::~RDOStudioEditBaseView()
{
	eraseMenu( &popupMenu );
}

BOOL RDOStudioEditBaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !RDOStudioView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

int RDOStudioEditBaseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOStudioView::OnCreate(lpCreateStruct) == -1 ) return -1;

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	rbool maximized = studioApp.m_pMainFrame->isMDIMaximazed();
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu, 4 + delta, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 3, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 5, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 3, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );

	return 0;
}

void RDOStudioEditBaseView::OnDraw(CDC* /*pDC*/)
{
	RDOStudioEditBaseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL RDOStudioEditBaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioEditBaseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RDOStudioEditBaseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RDOStudioEditBaseView::AssertValid() const
{
	RDOStudioView::AssertValid();
}

void RDOStudioEditBaseView::Dump(CDumpContext& dc) const
{
	RDOStudioView::Dump(dc);
}

RDOStudioEditBaseDoc* RDOStudioEditBaseView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioEditBaseDoc)));
	return (RDOStudioEditBaseDoc*)m_pDocument;
}
#endif

void RDOStudioEditBaseView::restartBufTimer( const int bufIndex )
{
	rbool canClear = studioApp.m_pMainFrame->style_editor.buffer->canClearBuffer;
	int   delay    = studioApp.m_pMainFrame->style_editor.buffer->clearBufferDelay * 1000;
	if ( delay < 0 ) delay = 0;
	switch ( bufIndex ) {
		case 1: stopTimer( timerBuf1 ); if ( canClear ) timerBuf1 = SetTimer( timerBuf1_ID, delay, NULL ); break;
		case 2: stopTimer( timerBuf2 ); if ( canClear ) timerBuf2 = SetTimer( timerBuf2_ID, delay, NULL ); break;
		case 3: stopTimer( timerBuf3 ); if ( canClear ) timerBuf3 = SetTimer( timerBuf3_ID, delay, NULL ); break;
		case 4: stopTimer( timerBuf4 ); if ( canClear ) timerBuf4 = SetTimer( timerBuf4_ID, delay, NULL ); break;
	}
}

void RDOStudioEditBaseView::stopTimer( UINT& timer )
{
	if ( timer ) {
		KillTimer( timer );
		timer = 0;
	}
}

void RDOStudioEditBaseView::OnDestroy() 
{
	RDOStudioView::OnDestroy();
	
	stopTimer( timerBuf1 );
	stopTimer( timerBuf2 );
	stopTimer( timerBuf3 );
	stopTimer( timerBuf4 );
}

void RDOStudioEditBaseView::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == timerBuf1 ) {
		resetBuf1 = true;
		stopTimer( timerBuf1 );
	} else if ( nIDEvent == timerBuf2 ) {
		resetBuf2 = true;
		stopTimer( timerBuf2 );
	} else if ( nIDEvent == timerBuf3 ) {
		resetBuf3 = true;
		stopTimer( timerBuf3 );
	} else if ( nIDEvent == timerBuf4 ) {
		resetBuf4 = true;
		stopTimer( timerBuf4 );
	}
	
	RDOStudioView::OnTimer(nIDEvent);
}

RDOEditorEdit* RDOStudioEditBaseView::getEdit() const
{
	return NULL;
}
