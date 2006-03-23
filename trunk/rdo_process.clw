; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=RPShapeTerminateDlg1_MJ
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rdo_process.h"
LastPage=0

ClassCount=18
Class1=RPDockWnd
Class2=RPPageCtrl
Class3=RPPageCtrlItemLabel
Class4=RPPageCtrlItem
Class5=RPProjectBar
Class6=CSizingControlBarG
Class7=CSizingControlBar
Class8=CSCBMiniDockFrameWnd
Class9=RPApp
Class10=RPAboutDlg
Class11=RPChildFrame
Class12=RPFlowChart
Class13=RPMainFrame

ResourceCount=9
Resource1=IDD_DIALOG_TERMINATE_1_MJ
Resource2=IDR_RDO_PRTYPE (English (U.S.))
Resource3=IDD_ABOUTBOX (English (U.S.))
Resource4=IDD_ABOUT
Class14=RPShapeCreateDlg1_MJ
Resource5=IDD_DIALOG_CREATE_1_MJ
Class15=RPShapeCreateDlg2_MJ
Resource6=IDD_DIALOG_PROCESS_2_MJ
Class16=RPShapeProcessDlg1_MJ
Resource7=IDD_DIALOG_CREATE_2_MJ
Class17=RPShapeProcessDlg2_MJ
Resource8=IDD_DIALOG_PROCESS_1_MJ
Class18=RPShapeTerminateDlg1_MJ
Resource9=IDR_MAINFRAME (English (U.S.))

[CLS:RPDockWnd]
Type=0
BaseClass=CSizingControlBarG
HeaderFile=ctrl\rdoprocess_dockwnd.h
ImplementationFile=ctrl\rdoprocess_dockwnd.cpp

[CLS:RPPageCtrl]
Type=0
BaseClass=CWnd
HeaderFile=ctrl\rdoprocess_pagectrl.h
ImplementationFile=ctrl\rdoprocess_pagectrl.cpp

[CLS:RPPageCtrlItemLabel]
Type=0
BaseClass=CWnd
HeaderFile=ctrl\rdoprocess_pagectrl.h
ImplementationFile=ctrl\rdoprocess_pagectrl.cpp

[CLS:RPPageCtrlItem]
Type=0
BaseClass=CWnd
HeaderFile=ctrl\rdoprocess_pagectrl.h
ImplementationFile=ctrl\rdoprocess_pagectrl.cpp
Filter=W
VirtualFilter=WC
LastObject=ID_APP_ABOUT

[CLS:RPProjectBar]
Type=0
BaseClass=RPDockWnd
HeaderFile=ctrl\rdoprocess_projectbar.h
ImplementationFile=ctrl\rdoprocess_projectbar.cpp

[CLS:CSizingControlBarG]
Type=0
BaseClass=baseCSizingControlBarG
HeaderFile=ctrl\sizecbar\scbarg.h
ImplementationFile=ctrl\sizecbar\scbarg.cpp

[CLS:CSizingControlBar]
Type=0
BaseClass=baseCSizingControlBar
HeaderFile=ctrl\sizecbar\sizecbar.h
ImplementationFile=ctrl\sizecbar\sizecbar.cpp

[CLS:CSCBMiniDockFrameWnd]
Type=0
BaseClass=baseCSCBMiniDockFrameWnd
HeaderFile=ctrl\sizecbar\sizecbar.h
ImplementationFile=ctrl\sizecbar\sizecbar.cpp
Filter=N
LastObject=CSCBMiniDockFrameWnd

[CLS:RPApp]
Type=0
BaseClass=CWinApp
HeaderFile=rdoprocess_app.h
ImplementationFile=rdoprocess_app.cpp

[CLS:RPChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=rdoprocess_childfrm.h
ImplementationFile=rdoprocess_childfrm.cpp
Filter=M
LastObject=ID_APP_ABOUT

[CLS:RPFlowChart]
Type=0
BaseClass=CWnd 
HeaderFile=rdoprocess_flowchart.h
ImplementationFile=rdoprocess_flowchart.cpp
Filter=W
VirtualFilter=WC
LastObject=RPFlowChart

[CLS:RPMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=rdoprocess_mainfrm.h
ImplementationFile=rdoprocess_mainfrm.cpp

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_FLOW_SELECT
Command6=ID_FLOW_CONNECTOR
Command7=ID_FLOW_ROTATE
CommandCount=7

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_RDO_PRTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_CLOSE
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_WINDOW_CASCADE
Command11=ID_WINDOW_TILE_HORZ
Command12=ID_WINDOW_ARRANGE
Command13=ID_APP_ABOUT
CommandCount=13

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_EDIT_PASTE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_NEXT_PANE
Command7=ID_PREV_PANE
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_CUT
Command11=ID_EDIT_UNDO
CommandCount=11

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ABOUT]
Type=1
Class=?
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_ABOUT_PICTURE,static,1342177283
Control3=IDC_ABOUT_CAPTION,static,1342308353
Control4=IDC_ABOUT_ADDRESS,static,1342308353
Control5=IDC_ABOUT_TEL,static,1342308354
Control6=IDC_ABOUR_EMAIL_STATIC,static,1342308354
Control7=IDC_ABOUT_EMAIL,static,1342308608
Control8=IDC_ABOUR_WEB_STATIC,static,1342308354
Control9=IDC_ABOUT_WEB,static,1342308608

[DLG:IDD_DIALOG_CREATE_1_MJ]
Type=1
Class=RPShapeCreateDlg1_MJ
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_COMBO1,combobox,1344340227
Control12=IDC_EDIT4,edit,1350631552
Control13=IDC_EDIT5,edit,1484849280
Control14=IDC_EDIT6,edit,1476460672
Control15=IDC_EDIT7,edit,1476460672
Control16=IDC_EDIT8,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_BUTTON1,button,1342242816

[CLS:RPShapeCreateDlg1_MJ]
Type=0
HeaderFile=rdoprocess_shape_create_dlg1_MJ.h
ImplementationFile=rdoprocess_shape_create_dlg1_MJ.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_APP_ABOUT

[DLG:IDD_DIALOG_CREATE_2_MJ]
Type=1
Class=RPShapeCreateDlg2_MJ
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350631552
Control10=IDC_EDIT4,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT5,edit,1350631552
Control13=IDC_STATIC,button,1342177287

[CLS:RPShapeCreateDlg2_MJ]
Type=0
HeaderFile=rdoprocess_shape_create_dlg2_MJ.h
ImplementationFile=rdoprocess_shape_create_dlg2_MJ.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DIALOG_PROCESS_1_MJ]
Type=1
Class=RPShapeProcessDlg1_MJ
ControlCount=30
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO1,combobox,1344340227
Control6=IDC_EDIT4,edit,1350631552
Control7=IDC_EDIT5,edit,1484849280
Control8=IDC_EDIT6,edit,1476460672
Control9=IDC_EDIT7,edit,1476460672
Control10=IDC_EDIT8,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT1,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_COMBO2,combobox,1344340227
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT2,edit,1350631552
Control22=IDC_STATIC,button,1342177287
Control23=IDC_BUTTON1,button,1342242816
Control24=IDC_BUTTON2,button,1342242816
Control25=IDC_LIST1,listbox,1352728835
Control26=IDC_STATIC,static,1342308352
Control27=IDC_COMBO3,combobox,1344340227
Control28=IDC_STATIC,button,1342177287
Control29=IDC_STATIC,static,1342308352
Control30=IDC_COMBO4,combobox,1344340227

[CLS:RPShapeProcessDlg1_MJ]
Type=0
HeaderFile=rdoprocess_shape_process_dlg1_MJ.h
ImplementationFile=rdoprocess_shape_process_dlg1_MJ.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT
VirtualFilter=dWC

[DLG:IDD_DIALOG_PROCESS_2_MJ]
Type=1
Class=RPShapeProcessDlg2_MJ
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO1,combobox,1344340226

[CLS:RPShapeProcessDlg2_MJ]
Type=0
HeaderFile=rdoprocess_shape_process_dlg2_MJ.h
ImplementationFile=rdoprocess_shape_process_dlg2_MJ.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT

[DLG:IDD_DIALOG_TERMINATE_1_MJ]
Type=1
Class=RPShapeTerminateDlg1_MJ
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:RPShapeTerminateDlg1_MJ]
Type=0
HeaderFile=rdoprocess_shape_terminate_dlg1_MJ.h
ImplementationFile=rdoprocess_shape_terminate_dlg1_MJ.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

