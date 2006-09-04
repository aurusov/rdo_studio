# Microsoft Developer Studio Project File - Name="rp_method" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rp_method - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rp_method.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rp_method.mak" CFG="rp_method - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rp_method - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "rp_method - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "$(SolutionDir)$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /I "../rp_misc" /I "../rp_ctrl" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /YX /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "../rp_misc" /I "../rp_ctrl" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /Yu"stdafx.h" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Debug\rp_method.lib"

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "$(SolutionDir)$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /GX /Ob2 /I "../rp_misc" /I "../rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /GX /Ob2 /I "../rp_misc" /I "../rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Release\rp_method.lib"

!ENDIF 

# Begin Target

# Name "rp_method - Win32 Debug"
# Name "rp_method - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\rdoprocess_connector.cpp
DEP_CPP_RDOPR=\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_factory.cpp
DEP_CPP_RDOPRO=\
	"..\rp_ctrl\rdoprocess_toolbar.h"\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_pixmap.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	".\block_default.xpm"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_method.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_flowchart.cpp
DEP_CPP_RDOPROC=\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_flowchart.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_messages.cpp
DEP_CPP_RDOPROCE=\
	"..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_method.cpp
DEP_CPP_RDOPROCES=\
	"..\rp_ctrl\rdoprocess_toolbar.h"\
	"..\rp_misc\rdoprocess_pixmap.h"\
	"..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_method.h"\
	".\rdoprocess_object.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object.cpp
DEP_CPP_RDOPROCESS=\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_project.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_chart.cpp
DEP_CPP_RDOPROCESS_=\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_flowchart.cpp
DEP_CPP_RDOPROCESS_O=\
	"..\rp_ctrl\rdoprocess_toolbar.h"\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_pixmap.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_flowchart.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_method.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_object_matrix.cpp
DEP_CPP_RDOPROCESS_OB=\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_project.cpp
DEP_CPP_RDOPROCESS_P=\
	"..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_project.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape.cpp
DEP_CPP_RDOPROCESS_S=\
	"..\rp_misc\rdoprocess_math.h"\
	"..\rp_misc\rdoprocess_matrix.h"\
	"..\rp_misc\rdoprocess_point.h"\
	"..\rp_misc\rdoprocess_polyline.h"\
	"..\rp_misc\rdoprocess_rect.h"\
	"..\rp_misc\rdoprocess_string.h"\
	"..\rp_misc\rdoprocess_xml.h"\
	".\rdoprocess_connector.h"\
	".\rdoprocess_factory.h"\
	".\rdoprocess_messages.h"\
	".\rdoprocess_object.h"\
	".\rdoprocess_object_chart.h"\
	".\rdoprocess_object_flowchart.h"\
	".\rdoprocess_object_matrix.h"\
	".\rdoprocess_project.h"\
	".\rdoprocess_shape.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	"..\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_method - Win32 Debug"

# ADD CPP /nologo /GX /Yc"stdafx.h" /GZ

!ELSEIF  "$(CFG)" == "rp_method - Win32 Release"

# ADD CPP /nologo /GX /Yc"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\rdoprocess_connector.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_factory.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_flowchart.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_messages.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_method.h
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

SOURCE=.\rdoprocess_project.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# End Target
# End Project
