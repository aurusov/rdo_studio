#include "stdafx.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_app.h"
#include "rdoprocess_string.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include "resource.h"
#include "rdoprocess_object.h"

#include "rdoprocess_generation_type_MJ.h" // MJ диалоговое окно кнопки типа генерирования на тулбаре

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RPMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RPMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RPMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FLOW_CONNECTOR, OnUpdateFlowConnector)
	ON_UPDATE_COMMAND_UI(ID_FLOW_ROTATE, OnUpdateFlowRotate)
	ON_UPDATE_COMMAND_UI(ID_FLOW_SELECT, OnUpdateFlowSelect)
	ON_COMMAND(ID_FLOW_SELECT, OnFlowSelect)
	ON_COMMAND(ID_FLOW_ROTATE, OnFlowRotate)
	ON_COMMAND(ID_FLOW_CONNECTOR, OnFlowConnector)
	ON_COMMAND(ID_GENERATE, OnGenerate)
	ON_COMMAND(ID_GEN_TYPE, OnGenType)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_BRUSH, OnUpdateBtnFillBrush)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_PEN, OnUpdateBtnFillBrush)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_FONT, OnUpdateBtnFillBrush)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

RPMainFrame::RPMainFrame()
{
}

RPMainFrame::~RPMainFrame()
{
}

int RPMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndToolBar.LoadToolBar( IDR_MAINFRAME );
	m_wndToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	m_wndToolBar.SetWindowText( rp::string::format( IDS_TOOLBAR_MAIN ).c_str() );

	m_wndStyleAndColorToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY );
	m_wndStyleAndColorToolBar.LoadToolBar( IDR_STYLEANDCOLOR );
	m_wndStyleAndColorToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	m_wndStyleAndColorToolBar.SendMessage( TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS );
	m_wndStyleAndColorToolBar.SetWindowText( rp::string::format( IDS_TOOLBAR_STYLEANDCOLOR ).c_str() );

	int index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_BRUSH );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_PEN );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_FONT );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );

	//MJ start 02.04.06
	m_wndToolBlockBarMJ.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	m_wndToolBlockBarMJ.LoadToolBar( TOOLBARBLOCKMJ );
	m_wndToolBlockBarMJ.ModifyStyle( 0, TBSTYLE_FLAT );
	//MJstop

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	projectBar.Create( rp::string::format( ID_DOCK_PROJECT_BAR ).c_str(), this, 0 );
	projectBar.EnableDocking( CBRS_ALIGN_ANY );

	CListCtrl* listctrl = new CListCtrl();
	listctrl->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl->InsertItem( 0, "test 1" );
	listctrl->InsertItem( 1, "test 2" );
	listctrl->InsertItem( 2, "test 3" );
	listctrl->InsertItem( 3, "test 4" );
	projectBar.insertPage( listctrl, "Первая страница" );

	CListCtrl* listctrl2 = new CListCtrl();
	listctrl2->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl2->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl2->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl2->InsertItem( 0, "q 1" );
	listctrl2->InsertItem( 1, "qqqqqqqqqqq 2" );
	listctrl2->InsertItem( 2, "q 3" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	projectBar.insertPage( listctrl2, "Вторая страница" );

	CListCtrl* listctrl3 = new CListCtrl();
	listctrl3->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl3->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl3->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl3->InsertItem( 0, "объект 1" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	projectBar.insertPage( listctrl3, "Третья страница" );

	projectBar.selectFirst();

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndStyleAndColorToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBlockBarMJ.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &m_wndToolBar );
	dockControlBarBesideOf( m_wndStyleAndColorToolBar, m_wndToolBar );
	dockControlBarBesideOf( m_wndToolBlockBarMJ, m_wndStyleAndColorToolBar );
	DockControlBar( &projectBar, AFX_IDW_DOCKBAR_LEFT );

	return 0;
}

void RPMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
{
	RecalcLayout( TRUE );

	CRect rect;
	baseBar.GetWindowRect( rect );

	DWORD dw = baseBar.GetBarStyle();

	UINT n = 0;
	n = ( dw & CBRS_ALIGN_TOP          ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = ( dw & CBRS_ALIGN_BOTTOM && !n ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = ( dw & CBRS_ALIGN_LEFT   && !n ) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = ( dw & CBRS_ALIGN_RIGHT  && !n ) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	int dx = 0;
	int dy = 0;
	if ( n == AFX_IDW_DOCKBAR_TOP || n == AFX_IDW_DOCKBAR_BOTTOM ) dx = 1;
	if ( n == AFX_IDW_DOCKBAR_LEFT || n == AFX_IDW_DOCKBAR_RIGHT ) dy = 1;

	rect.OffsetRect( dx, dy );

	DockControlBar( &bar, n, rect );
}

BOOL RPMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

#ifdef _DEBUG
void RPMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void RPMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif

void RPMainFrame::OnUpdateFlowSelect( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_select );
}

void RPMainFrame::OnUpdateFlowConnector( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_connector );
}

void RPMainFrame::OnUpdateFlowRotate( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_rotate );
}

void RPMainFrame::OnFlowSelect()
{
	rpapp.project().setFlowState( RPProject::flow_select );
}

void RPMainFrame::OnFlowConnector()
{
	rpapp.project().setFlowState( RPProject::flow_connector );
}

void RPMainFrame::OnFlowRotate()
{
	rpapp.project().setFlowState( RPProject::flow_rotate );
}

	// MJ start 29.03.06 обработчик кнопки генерирования
void RPMainFrame::OnGenerate() 
{
	
	rpapp.RDOfiles->pattern.open("aaa\\RDO_PROCESS.pat");
	rpapp.RDOfiles->resourse.open("aaa\\RDO_PROCESS.rss");
	rpapp.RDOfiles->function.open("aaa\\RDO_PROCESS.fun");
	
	rpapp.RDOfiles->typeres.open("aaa\\RDO_PROCESS.rtp");
	rpapp.RDOfiles->operations.open("aaa\\RDO_PROCESS.opr");
	rpapp.RDOfiles->smr.open("aaa\\RDO_PROCESS.smr");
	rpapp.RDOfiles->statistic.open("aaa\\RDO_PROCESS.pmd");
	
	blank_rdo_MJ();
	rpapp.project().list_name();
	
	rpapp.RDOfiles->pattern.close();
	
	rpapp.RDOfiles->resourse<<endl<<endl<<"$End";
	rpapp.RDOfiles->resourse.close();
	
	rpapp.RDOfiles->function.close();
	rpapp.RDOfiles->typeres.close();
	
	
	rpapp.RDOfiles->smr.close();
	
	
	
	rpapp.RDOfiles->statistic<<
	endl<<"$End";
	rpapp.RDOfiles->statistic.close();
	
	
	//ГЕНЕРИРОВАНИЕ  ОПЕРАЦИЙ *.opr тут т.к. должны быть уже сгенерированны все файлы

		rpapp.project().list_pattern_names.push_back("Блок_останова_моделирования_по_времени");

					rpapp.RDOfiles->operations<<"$Operations";

				std::list<CString>::iterator it = rpapp.project().list_pattern_names.begin();
				  int amount = 1;	
				
				  while( it != rpapp.project().list_pattern_names.end() ) 
					{
					TRACE( "%s\n", (*it) );
					rpapp.RDOfiles->operations<<endl
					<<endl<<"operation_"<<amount<<" : "<<(*it);

					it++; amount++;
					}


					rpapp.RDOfiles->operations
					<<endl<<"$End";
rpapp.project().list_pattern_names.clear();

	rpapp.RDOfiles->operations.close();
	
	
	
	
	

	
}
	// MJ stop

void RPMainFrame::blank_rdo_MJ()
{
//ГЕНЕРИРОВАНИЕ ТИПОВ РЕССУРСОВ *.rtp

rpapp.RDOfiles->typeres
<<endl	
<<endl<<"{ res }"
<<endl<<"$Resource_type Resource : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"Состояние : (занят,свободен)"
<<endl<<"Колличество_транзактов : integer {сколько обслуживается транзактов в текущий момент}"
<<endl<<"Максим_колличество : integer {макс колличество которое может обслуживаться"
<<endl<<"после которого рессурс переходит в состяоние занят}"
<<endl<<"$End"
<<endl
<<endl
<<endl
<<endl
<<endl
<<endl<<"{1}"
<<endl<<"$Resource_type Creates : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"  par_1 : (true, false) {false} {связанно с созданием первого только}"
<<endl<<"  par_amount: integer   {1}     {соклько выполнилось раз} "
<<endl<<"$End"
<<endl
<<endl
<<endl
<<endl<<"$Resource_type Group_of_transacts_X : temporary    {Параметры создаваемого транзакта}"
<<endl<<"$Parameters"
<<endl<<"   {параметры транзакта задаются пользователем"
<<endl<<"    и генерируются автоматически}"
<<endl
<<endl<<"{ ПАРАМЕТРЫ МЕСТО НАХОЖДЕНИЯ }"
<<endl
<<endl<<"	Место_нахождения :(";

rpapp.project().list_name_for_type_res(); // ф-ия заполняет имнами всех блоков
rpapp.RDOfiles->typeres<<"ОШИБКА_НАДО_УСТРАНИТЬ)"


<<endl<<"	Место_нахождения_будущее : such_as  Group_of_transacts_X.Место_нахождения {ID модуля, куда планируется отправится на след шаге}"
<<endl
<<endl	
<<endl
<<endl<<"{ CОСТОЯНИЕ ТРАНЗАКТА}	"
<<endl<<"	Состояние_транзакта : integer {}"
<<endl
<<endl
<<endl<<"{PRIMARY KEY}"
<<endl<<"    Идентификационный_номер_транзакта : integer"
<<endl<<"    Идентификационный_номер_группы_транзактов : such_as  Group_of_transacts_X.Место_нахождения"
<<endl<<"{-----------}"
<<endl
<<endl<<"	Место_в_очереди : integer"
<<endl<<"	Состояние_транспортировки : (ожидает, захвачен, в_очереди, в_очереди_выход)"
<<endl
<<endl<<"{ ДОПОЛНИТЕЛЬНЫЕ ПАРАМЕТРЫ ОПРЕДЕЛЯЮЩИЕСЯ ПОЛЬЗОВАТЕЛЕМ}"
<<endl
<<endl<<"{" 
<<endl<<"... : ..."
<<endl<<" }"
<<endl<<"$End "
<<endl 
<<endl<<" {2 очередь}"
<<endl<<" $Resource_type Queue : permanent"
<<endl<<"$Parameters"
<<endl<<"  Размер_очереди : integer"
<<endl<<"	 Выход_очереди: (свободен, занят)"
<<endl<<"$End"
<<endl
<<endl
<<endl<<"{3 процесс}"
<<endl<<"$Resource_type Block_process : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"  Состояние : (занят,свободен)"
<<endl<<"$End"
<<endl
<<endl<<"{4del}"
<<endl<<" $Resource_type Block_Del : permanent"
<<endl<<"$Parameters"
<<endl<<"  Колличество_удаленных : integer"
<<endl
<<endl<<"$End";


//ГЕНЕРИРОВАНИЕ  РЕССУРСОВ *.rtp
rpapp.RDOfiles->resourse<<"$Resources";


//ГЕНЕРИРОВАНИЕ  СТАТИСТИКИ *.pmd
rpapp.RDOfiles->statistic<<

endl<<"$Results";



//ГЕНЕРИРОВАНИЕ  SMR *.smr
rpapp.RDOfiles->smr<<
endl<<"Model_name = RDO_PROCESS"
<<endl
<<endl<<"Resource_file  = RDO_PROCESS"
<<endl<<"OprIev_file    = RDO_PROCESS" // OPR
<<endl<<"Trace_file     = RDO_PROCESS"
<<endl<<"Statistic_file = RDO_PROCESS" //PMD
<<endl<<"Results_file   = RDO_PROCESS"
//<<endl<<"Frame_file     = RDO_PROCESS"
//<<endl<<"Frame_number   = 1"
//<<endl<<"Show_mode      = Animation"
//<<endl<<"Show_rate      = 10000000.0"
<<endl
<<endl<<"Terminate_if  Time_now >= "<< rpapp.project().generate_time_MJ;



// ГЕНЕРИРОВАНИЕ ОПРЕЦИЙ в *.pat генерация блока перемещения
rpapp.project().list_pattern_names.push_back("Блок_перемещения");// добавляем имя для *.opr

rpapp.RDOfiles->pattern
<<"{----------------ГЕНЕРАЦИЯ БЛОКА ПЕРЕМЕЩЕНИЯ-------------------------------------------}"
<<endl<<"$Pattern Блок_перемещения : rule  {срабатывание закона}trace"
<<endl<<"	$Relevant_resources"
<<endl<<"		_transact_X : Group_of_transacts_X Keep"
<<endl
<<endl<<"	$Body"
<<endl
<<endl<<"	_transact_X"
<<endl<<"		Choice from _transact_X.Состояние_транспортировки = ожидает  and"
<<endl<<"		_transact_X.Место_нахождения_будущее<>_transact_X.Место_нахождения "
<<endl<<"	first"
<<endl
<<endl<<"	Convert_rule"
<<endl<<"		Место_нахождения set  _transact_X.Место_нахождения_будущее"
<<endl
<<endl<<"	$End";


rpapp.RDOfiles->pattern
<<endl<<"{----------------ГЕНЕРАЦИЯ если останавливается по времени-------------------------------------------}"
<<endl
<<endl<<"$Pattern  Блок_останова_моделирования_по_времени : irregular_event trace "
<<endl<<"$Relevant_resources"
<<endl<<"_parameter : Group_of_transacts_X  NoChange {нужен для того, что без него глючит}"
<<endl<<"$Time ="<< rpapp.project().generate_time_MJ
<<endl<<"$Body"
<<endl<<"_parameter"
<<endl<<"$End";



}

void RPMainFrame::OnGenType() 
{
	RP_GENERATION_TYPE_MJ dlg;
	dlg.DoModal();	
}

void RPMainFrame::OnUpdateBtnFillBrush( CCmdUI* pCmdUI )
{
	pCmdUI->Enable();
}
