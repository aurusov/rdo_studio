# Microsoft Developer Studio Project File - Name="rdo_process" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rdo_process - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_process.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_process.mak" CFG="rdo_process - Win32 Release"
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

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /Ob2 /I "./rp_misc" /I "./rp_method" /I "./rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /YX /GF /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /I "./rp_misc" /I "./rp_method" /I "./rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o ".\Release\rdo_process.bsc"
# ADD BSC32 /nologo /o ".\Release\rdo_process.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib $(SolutionDir)$(ConfigurationName)\rp_misc.lib $(SolutionDir)$(ConfigurationName)\rp_method.lib /nologo /subsystem:windows /machine:IX86 /out:".\Release\$(ProjectName).exe" /pdbtype:sept
# ADD LINK32 .\release\rp_ctrl.lib .\release\rp_misc.lib .\release\rp_method.lib .\release\method_algorithm.lib .\release\method_proc2rdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib /nologo /subsystem:windows /machine:IX86 /pdbtype:sept

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "./rp_misc" /I "./rp_method" /I "./rp_ctrl" /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /D "NO_ANSIUNI_ONLY" /D "_AFXDLL" /YX /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "./rp_misc" /I "./rp_method" /I "./rp_ctrl" /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /D "NO_ANSIUNI_ONLY" /D "_AFXDLL" /Yu"stdafx.h" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o ".\Debug\rdo_process.bsc"
# ADD BSC32 /nologo /o ".\Debug\rdo_process.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib $(SolutionDir)$(ConfigurationName)\rp_misc.lib $(SolutionDir)$(ConfigurationName)\rp_method.lib /nologo /subsystem:windows /pdb:"$(TargetDir)$(TargetName).pdb" /debug /machine:IX86 /out:".\Debug\$(ProjectName).exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 .\debug\rp_ctrl.lib .\debug\rp_misc.lib .\debug\rp_method.lib .\debug\method_algorithm.lib .\debug\method_proc2rdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "rdo_process - Win32 Release"
# Name "rdo_process - Win32 Debug"
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Group "controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=ctrl\rdolink.cpp
DEP_CPP_RDOLI=\
	".\ctrl\rdolink.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\rdolink.h
# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_dockwnd.cpp
DEP_CPP_RDOPR=\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_dockwnd.h
# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_pagectrl.cpp
DEP_CPP_RDOPRO=\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_misc\mctranspblt\McTransparentBlit.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_pagectrl.h
# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_projectbar.cpp
DEP_CPP_RDOPROC=\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\ctrl\rdoprocess_projectbar.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_projectbar.h
# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_toolbar.cpp
DEP_CPP_RDOPROCE=\
	".\ctrl\rdoprocess_toolbar.h"\
	".\rdoprocess_project.h"\
	".\rp_ctrl\rdoprocess_toolbar.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\rdoprocess_toolbar.h
# End Source File
# End Group
# Begin Group "SizeCBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=ctrl\sizecbar\scbarg.cpp
DEP_CPP_SCBAR=\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\sizecbar\scbarg.h
# End Source File
# Begin Source File

SOURCE=ctrl\sizecbar\sizecbar.cpp
DEP_CPP_SIZEC=\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\sizecbar\sizecbar.h
# End Source File
# End Group
# Begin Group "ColourPicker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=ctrl\colourpicker\ColourPicker.cpp
DEP_CPP_COLOU=\
	".\ctrl\colourpicker\ColourPicker.h"\
	".\ctrl\colourpicker\ColourPopup.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\colourpicker\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=ctrl\colourpicker\ColourPopup.cpp
DEP_CPP_COLOUR=\
	".\ctrl\colourpicker\ColourPicker.h"\
	".\ctrl\colourpicker\ColourPopup.h"\
	".\rp_misc\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ctrl\colourpicker\ColourPopup.h
# End Source File
# End Group
# End Group
# Begin Group "RDOProcess"

# PROP Default_Filter ""
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=rdoprocess_app.cpp
DEP_CPP_RDOPROCES=\
	".\ctrl\rdolink.h"\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\ctrl\rdoprocess_projectbar.h"\
	".\ctrl\rdoprocess_toolbar.h"\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rdoprocess_app.h"\
	".\rdoprocess_childfrm.h"\
	".\rdoprocess_mainfrm.h"\
	".\rdoprocess_method_manager.h"\
	".\rdoprocess_project.h"\
	".\rp_ctrl\rdoprocess_toolbar.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_method.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=rdoprocess_childfrm.cpp
DEP_CPP_RDOPROCESS=\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rdoprocess_childfrm.h"\
	".\rdoprocess_docview.h"\
	".\rp_method\rdoprocess_connector.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_flowchart.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_object_chart.h"\
	".\rp_method\rdoprocess_object_flowchart.h"\
	".\rp_method\rdoprocess_object_matrix.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_method\rdoprocess_shape.h"\
	".\rp_misc\rdoprocess_math.h"\
	".\rp_misc\rdoprocess_matrix.h"\
	".\rp_misc\rdoprocess_point.h"\
	".\rp_misc\rdoprocess_polyline.h"\
	".\rp_misc\rdoprocess_rect.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_docview.cpp
DEP_CPP_RDOPROCESS_=\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rdoprocess_docview.h"\
	".\rp_method\rdoprocess_connector.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_flowchart.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_object_chart.h"\
	".\rp_method\rdoprocess_object_flowchart.h"\
	".\rp_method\rdoprocess_object_matrix.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_method\rdoprocess_shape.h"\
	".\rp_misc\rdoprocess_math.h"\
	".\rp_misc\rdoprocess_matrix.h"\
	".\rp_misc\rdoprocess_point.h"\
	".\rp_misc\rdoprocess_polyline.h"\
	".\rp_misc\rdoprocess_rect.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=rdoprocess_mainfrm.cpp
DEP_CPP_RDOPROCESS_M=\
	".\ctrl\colourpicker\ColourPopup.h"\
	".\ctrl\rdolink.h"\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\ctrl\rdoprocess_projectbar.h"\
	".\ctrl\rdoprocess_toolbar.h"\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rdoprocess_app.h"\
	".\rdoprocess_childfrm.h"\
	".\rdoprocess_docview.h"\
	".\rdoprocess_mainfrm.h"\
	".\rdoprocess_method_manager.h"\
	".\rdoprocess_project.h"\
	".\rp_ctrl\rdoprocess_toolbar.h"\
	".\rp_method\rdoprocess_connector.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_method.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_object_chart.h"\
	".\rp_method\rdoprocess_object_flowchart.h"\
	".\rp_method\rdoprocess_object_matrix.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_method\rdoprocess_shape.h"\
	".\rp_misc\rdoprocess_math.h"\
	".\rp_misc\rdoprocess_matrix.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_point.h"\
	".\rp_misc\rdoprocess_polyline.h"\
	".\rp_misc\rdoprocess_rect.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_method_manager.cpp
DEP_CPP_RDOPROCESS_ME=\
	".\ctrl\rdolink.h"\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\ctrl\rdoprocess_projectbar.h"\
	".\ctrl\rdoprocess_toolbar.h"\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\method\algorithm\rdoprocess_method_algorithm.h"\
	".\method\proc2rdo\rdoprocess_creation_RDO_files_MJ.h"\
	".\method\proc2rdo\rdoprocess_method_proc2rdo_MJ.h"\
	".\method\proc2rdo\rdoprocess_shape_MJ.h"\
	".\rdoprocess_app.h"\
	".\rdoprocess_mainfrm.h"\
	".\rdoprocess_method_manager.h"\
	".\rp_ctrl\rdoprocess_toolbar.h"\
	".\rp_method\rdoprocess_connector.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_method.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_object_chart.h"\
	".\rp_method\rdoprocess_object_flowchart.h"\
	".\rp_method\rdoprocess_object_matrix.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_method\rdoprocess_shape.h"\
	".\rp_misc\rdoprocess_math.h"\
	".\rp_misc\rdoprocess_matrix.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_point.h"\
	".\rp_misc\rdoprocess_polyline.h"\
	".\rp_misc\rdoprocess_rect.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_project.cpp
DEP_CPP_RDOPROCESS_P=\
	".\ctrl\rdolink.h"\
	".\ctrl\rdoprocess_dockwnd.h"\
	".\ctrl\rdoprocess_pagectrl.h"\
	".\ctrl\rdoprocess_projectbar.h"\
	".\ctrl\rdoprocess_toolbar.h"\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rdoprocess_app.h"\
	".\rdoprocess_childfrm.h"\
	".\rdoprocess_docview.h"\
	".\rdoprocess_mainfrm.h"\
	".\rdoprocess_method_manager.h"\
	".\rdoprocess_project.h"\
	".\rp_ctrl\rdoprocess_toolbar.h"\
	".\rp_method\rdoprocess_connector.h"\
	".\rp_method\rdoprocess_factory.h"\
	".\rp_method\rdoprocess_messages.h"\
	".\rp_method\rdoprocess_method.h"\
	".\rp_method\rdoprocess_object.h"\
	".\rp_method\rdoprocess_object_chart.h"\
	".\rp_method\rdoprocess_object_flowchart.h"\
	".\rp_method\rdoprocess_object_matrix.h"\
	".\rp_method\rdoprocess_project.h"\
	".\rp_method\rdoprocess_shape.h"\
	".\rp_misc\rdoprocess_math.h"\
	".\rp_misc\rdoprocess_matrix.h"\
	".\rp_misc\rdoprocess_pixmap.h"\
	".\rp_misc\rdoprocess_point.h"\
	".\rp_misc\rdoprocess_polyline.h"\
	".\rp_misc\rdoprocess_rect.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\rp_misc\rdoprocess_xml.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\ctrl\sizecbar\scbarg.h"\
	".\ctrl\sizecbar\sizecbar.h"\
	".\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rdo_process - Win32 Release"

# ADD CPP /nologo /GX /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_process - Win32 Debug"

# ADD CPP /nologo /GX /Yc"stdafx.h" /GZ

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=rdoprocess_app.h
# End Source File
# Begin Source File

SOURCE=rdoprocess_childfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_docview.h
# End Source File
# Begin Source File

SOURCE=rdoprocess_mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_method_manager.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_project.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=res\flow_connector.cur
# End Source File
# Begin Source File

SOURCE=res\flow_doc_not.cur
# End Source File
# Begin Source File

SOURCE=res\flow_dock_in.cur
# End Source File
# Begin Source File

SOURCE=res\flow_dock_not.cur
# End Source File
# Begin Source File

SOURCE=res\flow_move.cur
# End Source File
# Begin Source File

SOURCE=res\flow_rotate.cur
# End Source File
# Begin Source File

SOURCE=res\flow_rotate_center.cur
# End Source File
# Begin Source File

SOURCE=res\flow_rotate_tl.cur
# End Source File
# Begin Source File

SOURCE=res\flow_scale_lr.cur
# End Source File
# Begin Source File

SOURCE=res\flow_scale_tb.cur
# End Source File
# Begin Source File

SOURCE=res\flow_scale_tlbr.cur
# End Source File
# Begin Source File

SOURCE=res\flow_scale_trbl.cur
# End Source File
# Begin Source File

SOURCE=res\flow_select.cur
# End Source File
# Begin Source File

SOURCE=.\res\flow_trash.cur
# End Source File
# Begin Source File

SOURCE=.\res\flowchart_default.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flowchart_icon.ico
# End Source File
# Begin Source File

SOURCE=res\pagectrl_close.bmp
# End Source File
# Begin Source File

SOURCE=res\pagectrl_close_over.bmp
# End Source File
# Begin Source File

SOURCE=res\pagectrl_page.bmp
# End Source File
# Begin Source File

SOURCE=res\rdo_process.ico
# End Source File
# Begin Source File

SOURCE=res\rdo_process.manifest
# End Source File
# Begin Source File

SOURCE=rdo_process.rc
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409
# End Source File
# Begin Source File

SOURCE=res\rdo_process.rc2
# End Source File
# Begin Source File

SOURCE=res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_empty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_mainframe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_mainframe_d.bmp
# End Source File
# Begin Source File

SOURCE=res\toolbar_styleandcolor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trash.bmp
# End Source File
# End Group
# End Group
# End Target
# End Project
