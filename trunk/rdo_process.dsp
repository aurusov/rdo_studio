# Microsoft Developer Studio Project File - Name="rdo_process" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rdo_process - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_process.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_process.mak" CFG="rdo_process - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_process - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rdo_process - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rdo_process - Win32 Release"
# Name "rdo_process - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdoprocess_app.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_childfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_connector.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_creation_RDO_files_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_flowchart.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_math.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_messages.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_chart.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_flowchart.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_point.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_polyline.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_project.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_rect.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_action.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_dlg1_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_dlg2_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_if.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_dlg1_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_dlg2_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_resource_DLG1_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_terminate_dlg1_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_terminate_MJ.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_string.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\rdoprocess_app.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_childfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_connector.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_creation_RDO_files_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_flowchart.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_math.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_matrix.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_messages.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_chart.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_flowchart.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_matrix.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_point.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_polyline.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_project.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_rect.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_action.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_dlg1_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_dlg2_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_create_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_if.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_dlg1_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_dlg2_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_process_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_resource.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_resource_DLG1_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_terminate_dlg1_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_terminate_MJ.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_string.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_connector.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_doc_not.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_dock_in.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_dock_not.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_rotate.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_rotate_center.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_rotate_tl.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_scale_lr.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_scale_tb.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_scale_tlbr.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_scale_trbl.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_select.cur
# End Source File
# Begin Source File

SOURCE=.\res\pagectrl_close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pagectrl_close_over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pagectrl_page.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rdo_process.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdo_process.manifest
# End Source File
# Begin Source File

SOURCE=.\rdo_process.rc
# End Source File
# Begin Source File

SOURCE=.\res\rdo_process.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Group "Ctrl Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ctrl\rdolink.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_dockwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_pagectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_projectbar.cpp
# End Source File
# End Group
# Begin Group "Ctrl Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ctrl\rdolink.h
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_dockwnd.h
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_pagectrl.h
# End Source File
# Begin Source File

SOURCE=.\ctrl\rdoprocess_projectbar.h
# End Source File
# End Group
# Begin Group "SizeCBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ctrl\sizecbar\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\sizecbar\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\ctrl\sizecbar\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\sizecbar\sizecbar.h
# End Source File
# End Group
# Begin Group "mctranspblt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ctrl\mctranspblt\McTransparentBlit.cpp
# End Source File
# Begin Source File

SOURCE=.\ctrl\mctranspblt\McTransparentBlit.h
# End Source File
# End Group
# End Group
# End Target
# End Project
